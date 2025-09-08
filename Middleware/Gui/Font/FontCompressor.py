import sys
import regex as re

def parse_c_code(c_code_string):
#    parser = c_parser.CParser()
#    ast = parser.parse(c_code_string, filename='<stdin>') # Use a dummy filename
    return ast


if (len(sys.argv) != 3):
  print("Error, expected 2 args")
  exit()

try:
  file = open(sys.argv[1], "r", encoding='utf-8')
except:
  print("Can't open Font file")
  exit()

cFileContent = file.read()

root = re.findall("const tFont Font20 = \\{ .* \\}\\;", cFileContent)

try:
   symboCnt = int(re.findall("\d{1,3}", re.findall(" \d{1,3}\\, ", root[0])[0])[0]);
except:
   print("Can't parce symbol count")
   exit()

print(symboCnt)

#"D:\Programing\MCU\ILI9341\ILI9341\Gui\Font\Font20.c"