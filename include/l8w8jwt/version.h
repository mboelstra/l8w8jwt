/*
   Copyright 2020 Raphael Beck

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

/**
 *  @file version.h
 *  @author Raphael Beck
 *  @brief l8w8jwt version checking.
 */

#ifndef L8W8JWT_VERSION_H
#define L8W8JWT_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Current l8w8jwt version number.
 */
#define L8W8JWT_VERSION 100

/**
 * Current l8w8jwt version number (as a human-readable string).
 */
#define L8W8JWT_VERSION_STR "1.0.0"

#if defined(_WIN32) && defined(L8W8JWT_DLL)
#ifdef L8W8JWT_BUILD_DLL
#define L8W8JWT_API __declspec(dllexport)
#else
#define L8W8JWT_API __declspec(dllimport)
#endif
#else
#define L8W8JWT_API
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif // L8W8JWT_VERSION_H
