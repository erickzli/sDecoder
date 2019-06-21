//
//  fill_decoder.hh
//
//  Created by Erick Li on 05/29/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __FILL_DECODER_H__
#define __FILL_DECODER_H__

#include <fstream>
#include <string>
#include <iostream>

/**
 * The main function of fill pattern decoder. It can decode the pattern of a
 *   fill symbol. A fill symbol describes areal geometries.
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param a double pointer pointing to the last byte of the data block
 */
int decodeFillPattern(char **cursor, std::string &jstring, int level,
                      char **tail);


/**
 * Decode the symbol flow, which is the largest and most essential pipeline in
 *   the model.
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param type of the fill decoder:
 *     0: Normal decoder;
 *     1: Mask symbol decoder.
 */
int decodeLayer(char **cursor, std::string &jstring, int level, int type);

/**
 * Decode the pattern of Simple Fill Symbol
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param type of the process:
 *     0: Normal process;
 *     1: Symbol process.
 */
int decodeSimpleFill(char **cursor, std::string &jstring, int level, int type);

/**
 * Decode the pattern of Line Fill Symbol
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 */
int decodeLineFill(char **cursor, std::string &jstring, int level);

/**
 * Decode the pattern of Marker Fill Symbol
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 */
int decodeMarkerFill(char **cursor, std::string &jstring, int level);

#endif
