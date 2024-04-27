#!/usr/bin/python3

import sys
import yaml
import re
import os
import subprocess
from collections import deque
from pathlib import Path
from PIL import Image
import itertools as iter_tools
from operator import itemgetter

def generateCImage(name: Path):
    filepath = name.with_suffix(".png")
    resizepath = filepath.with_suffix(".resize")
    print("filepath", filepath, "resizepath", resizepath)
    im = Image.open(filepath)
    width,height = im.size
    width = int((width+7)/8) * 8
    height = int((height+7)/8) * 8
    args=[ "-gravity=center", f"{filepath}", "-background", "yellow", "-extent", f"{width}x{height}", f"{resizepath}"]
    print(args)
    subprocess.run( args=args, executable="convert")
    subprocess.run( args=["--force", "--bpp=4", "--mode=sprites", f"{name}", f"{resizepath}"], executable="nin10kit" )

def rgbaToGBAColor(rgba):
    r,g,b,a = rgba
    if (a < 128): return 0
    return (r>>3, g>>3, b>>3)

def generateValidSpriteSize(size: dict):
    result = {}
    for dim in size:
        val = size[dim]
        if val <= 8: result[dim] = 8
        elif val <= 16: result[dim] = 16
        elif val <= 32: result[dim] = 32
        else: result[dim] = 64
    if result['x'] == 64 and result['y'] < 32: result['y'] = 32
    if result['y'] == 64 and result['x'] < 32: result['x'] = 32

    return result

def expandSpriteDataToSize(data: deque, size: dict):
    original_size = {'x': len(data[0]), 'y': len(data)}
    size_diff = {'x': size['x'] - original_size['x'], 'y': size['y'] - original_size['y']}

    left_pad = size_diff['x'] // 2
    right_pad = size_diff['x'] // 2 + (1 if size_diff['x'] % 2 == 1 else 0)

    top_pad = size_diff['y'] // 2
    bottom_pad = size_diff['y'] // 2 + (1 if size_diff['y'] % 2 == 1 else 0)

    # pad sides
    for row in data:
        row.extendleft([0 for _ in range(left_pad)])
        row.extend([0 for _ in range(right_pad)])
    data.extendleft([0 for __ in range(size['x'])] for _ in range(top_pad))
    data.extend([0 for __ in range(size['x'])] for _ in range(bottom_pad))

def convertDeque2DToList2D(d: deque):
    result = []
    for row in d:
        result.append(list(row))
    return result

def generateSpriteGFX(name: Path, bit_mode = "4bpp"):
    bit_mode = bit_mode.lower()
    if (bit_mode != "4bpp" and bit_mode != "8bpp"): 
        raise ValueError("bit_mode must be either '4bpp' or '8bpp'")
    
    inputpath = name.with_suffix(".png")
    im = Image.open(inputpath)

    if (im.width > 64 or im.height > 64):
        raise Exception(f"image at {inputpath} is too large to create sprite. Maximum size is 64x64")

    size = {'x': im.width, 'y': im.height}
    size = generateValidSpriteSize(size)

    sprite_data = {}
    sprite_data['size'] = size
    sprite_data['bit_mode'] = bit_mode
    

    img_data = im.load()

    data = deque()
    palette = [0]

    for i in range(im.height):
        row = deque()
        for j in range(im.width):
            color = rgbaToGBAColor(img_data[j, i])
            if (color == 0):
                row.append(0)
            else:
                count = palette.count(color)
                if count == 0:
                    colorIndex = len(palette)
                    palette.append(color)
                else:
                    colorIndex = palette.index(color)
                row.append(colorIndex)
        data.append(row)
    
    palette_limit = 16 if bit_mode == "4bpp" else 256

    if (len(palette) > palette_limit):
        raise Exception(f"Sprite cannot be generated from file {inputpath} because it exceeds the palette size")

    expandSpriteDataToSize(data, size)

    sprite_data['pixels'] = convertDeque2DToList2D(data)
    sprite_data['palette'] = palette

    sprite_data['type'] = "sprite"
    sprite_data['original_file'] = name.name
    
    return sprite_data
    
def importSprite(import_options: dict, source_path: Path, dest_path: Path):
    spritePngPath = None
    if "filename" in import_options:
        spritePngPath = source_path.with_name(data['filename'])
        if spritePngPath.suffix == "": spritePngPath = spritePngPath.with_suffix(".png")
    # trying import file name
    elif source_path.with_suffix(".png").exists():
        spritePngPath = source_path.with_suffix(".png")
    else:
        path_candidates = sorted(source_path.parent.glob("*.png"))
        if len(path_candidates) > 0:
            spritePngPath = path_candidates[0]
        else:
            raise FileNotFoundError(f"Could not find a valid png to generate sprite for {source_path}")
    out_data = generateSpriteGFX(spritePngPath)


    if "name" in import_options:
        out_data["name"] = import_options["name"]
    else:
        out_data["name"] = source_path.stem

    return out_data

def importAnimation(import_options: dict, source_path: Path, dest_path: Path):
    frame_base_name = None
    delim = None
    start_index = None
    if "base_filename" in import_options:
        frame_base_name = import_options["base_filename"]
    if "delim" in import_options:
        delim = import_options["delim"]
    if "start_index" in import_options:
        start_index = import_options["start_options"]
    
    # resolve any details of the filename pattern
    if frame_base_name == None or delim == None or start_index == None:
        
        delim_options = [" ", "_", "-"] if delim == None else [delim]
        start_index_options = [0, 1] if start_index == None else [start_index]
        base_path_option = frame_base_name if frame_base_name != None else source_path.stem

        potential_names = list(iter_tools.product(delim_options, start_index_options))
        potential_names = [{"delim": delim, "index": index, "path": source_path.with_name(base_path_option + delim + str(index)).with_suffix(".png")} for (delim, index) in potential_names]

        potential_names = sorted(potential_names, key=itemgetter("index", "delim") )

        for n in potential_names:
            if n["path"].exists():
                if delim == None: delim = n["delim"]
                if start_index == None: start_index = n["index"]
                if frame_base_name == None: frame_base_name = base_path_option
                break

        if frame_base_name == None:

            potential_syntax_options = list(iter_tools.product(delim_options, start_index_options))
            potential_syntax_options = sorted(potential_syntax_options, key=itemgetter(1, 0))

            for opt in potential_syntax_options:
                glob_str = "*" + opt[0] + str(opt[1]) +".png"
                validImages = sorted(source_path.parent.glob(glob_str))
                if (len(validImages) > 0):
                    if delim == None: delim = opt[0]
                    if start_index == None: start_index = opt[1]
                    if frame_base_name == None:
                        name = validImages[0].name
                        base_name_regex = re.compile(f'(.*){delim}{start_index}')
                        match = base_name_regex.match(name)
                        frame_base_name = match.group(1)


    frames = []
    frame_index = start_index
    while True:
        candidate_path = source_path.with_name(frame_base_name + delim + str(frame_index)).with_suffix(".png")
        if candidate_path.exists():
            frames.append(generateSpriteGFX(candidate_path))
        else:
            break
        frame_index += 1

    out_data = {}
    if "name" in import_options:
        out_data["name"] = import_options["name"]
    else:
        out_data["name"] = source_path.stem
    out_data["frames"] = frames
    out_data["type"] = "animation"

    return out_data

def import_asset(source_path: Path, dest_path: Path):
    with open(source_path) as stream:

        try:
            data = yaml.safe_load(stream)
        except yaml.YAMLError as exc:
            print("Failed to read file with the following error:")
            print(exc)
            exit(1)

    name = None
    if not "type" in data:
        raise Exception(f"import file {source_path} is missing a 'type' attribute, cannot read")
    import_type = data['type']

    if "name" in data:
        name = data["name"]
    else:
        name = source_path.stem
    print("Importing", name)

    out_data = None

    if import_type == "sprite":
        out_data = importSprite(data, source_path, dest_path)
    elif import_type == "animation":
        out_data = importAnimation(data, source_path, dest_path)
    else:
        raise Exception(f"unknown import type '{import_type}'")

    with open(dest_path, "w") as out_file:
        try:
            yaml.safe_dump(out_data, out_file)
        except yaml.YAMLError as exc:
            print(f"exporting {dest_path} failed")
            raise exc

def repaletteSprite(sprite: dict, old_pal: list, new_pal: list):
    pixels = sprite["pixels"]
    for row in pixels:
        for i in range(len(row)):
            row[i] = new_pal.index(old_pal[row[i]])
    sprite["palette"] = new_pal

def mergePalettes(dest_pal: list, source_pal: list, sprites: list):
    
    for color in source_pal:
        if color == 0: continue
        if not color in dest_pal:
            dest_pal.append(color)

    for spr in sprites:
        if spr["palette"] == source_pal:
            repaletteSprite(spr, source_pal, dest_pal)

    return dest_pal


def reducePalettes(original_palettes: list, sprites: list):
    if len(original_palettes) == 0:
        return []
    palette_len = 16 if len(original_palettes[0]) <= 16 else 256
    out_palettes = []
    for pal in original_palettes:
        merged = None
        for merge_pal in out_palettes:
            pal_set = set(tuple(pal))
            merge_set = set(tuple(merge_pal))
            if (pal_set.issubset(merge_set) or merge_set.issubset(pal_set) or len(merge_set.union(pal_set)) <= palette_len):
                #palettes can be merged
                merged = mergePalettes(merge_pal, pal, sprites)
                break
        #palettes cannot be merged
        if merged == None: out_palettes.append(pal)

    return out_palettes

def buildGraphicsFiles(inputs: list, output_path: Path):
    cpp_path = output_path.with_suffix(".cpp")
    hpp_path = output_path.with_suffix(".hpp")

    cpp_text = ""
    hpp_text = ""

    gfx_data = []
    for input in inputs:
        with open(input) as file:
            try:
                gfx = yaml.safe_load(file)
                gfx_data.append(gfx)
            except yaml.YAMLError as exc:
                print(f"reading {input} failed")
                raise exc

    anim_data = list(filter(lambda d: d["type"] == "animation", gfx_data))
    sprite_data = list(filter(lambda d: d["type"] == "sprite", gfx_data))
    total_sprites_list = sprite_data + [frame for anim in anim_data for frame in anim["frames"]]

    for sprite in total_sprites_list:
        sprite["palette"] = [tuple(color) if color != 0 else color for color in sprite["palette"]]
        print(sprite["palette"])

    spr_palettes = [spr["palette"] for spr in total_sprites_list]

    new_palettes = reducePalettes(spr_palettes, total_sprites_list)

    print(new_palettes)
    print(total_sprites_list[0]["palette"])


arg_iter = iter(sys.argv)

#skip script location
next(arg_iter)

command = next(arg_iter, None)

if command == "build":
    pass
    inputs = []
    output_path = None
    while True:
        next_arg = next(arg_iter, None)
        if next_arg == "-o":
            output_path = Path(next(arg_iter))
        elif next_arg == None:
            break
        else:
            new_input = Path(next_arg)
            if not new_input.exists():
                print(f"Input file {new_input} does not exist")
                exit(1)
            inputs.append(new_input)
    if output_path == None:
        output_path = Path(os.getcwd())

    buildGraphicsFiles(inputs, output_path)
    
    


elif command == "import":
    input = next(arg_iter, None)
    if input == None:
        print("No import file passed")
        exit(1)
    input_path = Path(input)
    if not input_path.exists():
        print("Import file does not exist")
        exit(1)
    output_path = next(arg_iter, None)
    if output_path == None:
        output_path = input_path.with_suffix(".gfx")
    else:
        output_path = Path(output_path)
        if output_path.is_dir():
            raise Exception(f"{output_path} is not a valid output path")
        
    import_asset(input_path, output_path)
    exit(0)
    

else:
    print(f"Unrecognized gba importer command {command}")
    exit(1)
