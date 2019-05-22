# sParser: A Parser for ArcGIS Binary Symbol Files 📇
sParser, s for symbol or style, is a parser dedicated for ArcGIS Symbol Files which are encoded in such a way that a parser is required to be understood.

## What does it do?
This tool can be used to parse a binary file which defines an ArcGIS symbol, and output the information in JSON format.

## What is the the input file?
The input file defines an ArcGIS symbol in a binary way. It is not guaranteed that all the "correct" .style file can be successfully parsed by this parser. (See known scenarios)

## How to get the input file?
After downloading the ArcGIS bundle, you may open the application called ArcMap. From the toolbar: Customize > Style Manager > Styles > Create New Style, you may create an Esri style file.

> this part has not yet finished...

## Any known scenarios that this tool cannot handle?
Yes. Due to the encoding method of the ArcGIS style file, some features are not supported so far. Including but not limited to:
1. RGB/HSV Color Space (partially supported)
  - When parsing color spaces encoded in RGB or HSV, only three fields each defined by double types can be gotten, while
  it is unknown what they represent.
2. Any features containing picture or 3D components;
3. Gradient Fill Symbol.
