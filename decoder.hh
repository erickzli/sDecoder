
//
//  decoder.hh
//
//  Created by Erick Li on 04/11/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __DECODER_H__
#define __DECODER_H__

#include "decoder_helper.hh"
#include "fill_decoder.hh"
#include "line_decoder.hh"
#include "marker_decoder.hh"
#include "json_writer.hh"

#include <stdlib.h>
#include <unistd.h>

#include <sstream>

const bool PRINT_TO_FILE = true;
const bool DO_REWIND = true;

const int USE_RGB = 0;
const int USE_HSV = 1;

/**
 * This function is the only one that should be used by external users. Users
 *   just need to read the binary file into memory, then provide two pointers
 *   pointing to the head and the tail of the data block on memory
 *   respectively. Then a jstring will be returned which contains the complete
 *   information about the given ArcGIS symbol.
 * \param a pointer pointing to the first bytes of the data block
 * \param a pointer pointing to the last byte of the data block
 * \param opton to if logging should be enabled.
 * \return the jstring in JSON format
 * WARNING: The input source MUST have an extra byte at the end of the block,
 *   and the num_of_bytes MUST include that byte. Otherwise, undefined
 *   behaviors may occur.
 *
 *  *cursor        *tail
 *     \             \
 *     |0|1|2|3|4|5|6|7|
 */
std::string grandDecoder(char *head, char *tail, bool enableLogging);


/**
 * Decode a color pattern. The color pattern defines the color proporty in
 *   ArcGIS symbols. ArcGIS provides three color spaces to users for defining
 *   colors--RGB, HSV, and CMYK. The colors are stored in 24-byte CIELab format
 *   if users define them in RGB/HSV, however, if users define the colors in
 *   CMYK format, they will be stored in a 4-byte format with each byte
 *   representing a component in CMYK.
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param Where this color is used on. Such as, "outline_color", etc.
 * NOTE: 1. ArcGIS uses CIELAB to represent RGB and HSV color.
 *   (https://support.esri.com/en/technical-article/000002092)
 *       2. The conversion to RGB/HSV is based on the knowledge from the
 *   websites: (https://www.cs.rit.edu/~ncs/color/t_convert.html#XYZ%20to%20CIE%20L*a*b*%20(CIELAB)%20&%20CIELAB%20to%20XYZ)
 * WARNING: The color model used by ArcGIS may have some problems in terms of
 *   the parameter they use for conversion between RGB/HSV used for user
 *   definition and CIELab used for storage.
 */
void decodeColorPattern(char **cursor, std::string &jstring, int level,
                        std::string color_type);

/**
 * Decodes the quantity of layers. A symbol can have one or multiple layers
 *   just like Photoshop or Affinity Designer. Each layer is independent to the
 *   others, and can be used for combining out a complex symbol.
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \return the quantity of layers
 * NOTE: If the number of layers decoded is not positive, the function will
 *   throw an exemption.
 */
int decodeLayerNumber(char **cursor, std::string &jstring, int level);

/**
 * Decode a double-precision floating-point formatted number. It takes 8 bytes
 *   for each of them.
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param the tag of the double-precision floating-point formatted number.
 * NOTE: this output of the result to the jstring will be converted based on
 *   the true decimal. For examples: 1.000 will be decoded as "0"; 4.300 will
 *   be decoded as "4.3"; 3.928273383 will be decoded as it is.
 */
double decodeDouble(char **cursor, std::string &jstring, int level,
                    std::string tag);

/**
 * Decode an integer. It takes 4 bytes for each of them.
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param the tag of the integer.
 */
int decodeInt(char **cursor, std::string &jstring, int level, std::string tag);

/**
 * Decode a string. It takes undefined size, since a string can have any length.
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param the tag of the string.
 * NOTE: This method so far just supports decoding the string for font name.
 */
int decodeString(char **cursor, std::string &jstring, int level,
                 std::string tag);

#endif
