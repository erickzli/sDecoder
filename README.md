# ArcGIS Style File Parser 📇
Used for parsing an ArcGIS binary file which defines symbols.

## What does it do?
This tool can be used to parse a binary file which defines an ArcGIS symbol, and output the information in JSON format.

## What about the the input file?
The input file should be correctly read by ArcMap under the extension ".style". It contains many information about the symbol
defined in ArcGIS. It is not guaranteed that all the "correct" .style file can be successfully parsed by this parser.

## Any known scenarios that this tool cannot handle?
Yes. Due to the encoding method of the ArcGIS style file, some features are not supported so far. Including but not limited to:
1. RGB/HSV Color Space (partially supported)
  - When parsing color spaces encoded in RGB or HSV, only three fields each defined by double types can be gotten, while
  it is unknown what they represent.
2. Any features containing picture or 3D components;
3. Gradient Fill Symbol.
