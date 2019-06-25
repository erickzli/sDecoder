//
//  marker_decoder.hh
//
//  Created by Erick Li on 04/16/19.
//  Copyright © 2019 Erick Li. All rights reserved.
//

#ifndef __MARKER_DECODER_H__
#define __MARKER_DECODER_H__

#include <fstream>
#include <string>
#include <iostream>


/**
 * The main function of marker pattern decoder.
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 */
int decodeMarkerPattern(char **cursor, std::string &jstring, int level);

/**
 * Decode the pattern of Simple Marker Symbol
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 */
int decodeSimpleMarker(char **cursor, std::string &jstring, int level);

/**
 * Decode the pattern of Character Marker Symbol
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 */
int decodeCharacterMarker(char **cursor, std::string &jstring, int level);

/**
 * Decode the pattern of Arrow Marker Symbol
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 */
int decodeArrowMarker(char **cursor, std::string &jstring, int level);

/**
 * Decode the types of markers
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 */
int decodeMarkerTypes(char **cursor, std::string &jstring, int level);

/**
 * Decode the types of masks
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 */
int decodeMaskTypes(char **cursor, std::string &jstring, int level);

/**
 * Decode the style of markers
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 * \param (See json_writer.hh)
 */
int decodeMarkerStyle(char **cursor, std::string &jstring, int level);

#endif
