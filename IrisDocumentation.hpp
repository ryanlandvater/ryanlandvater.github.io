/**
 * @file IrisCoreDox.hpp
 * @author Ryan Landvater
 * @brief Iris Documentation
 * @date 2024-10-11
 * 
 * @copyright Copyright (c) 2024
 * \mainpage
 * 
 * # Iris Official Application Programming Interface (API)
 * This documentation relates to the Iris whole slide imaging (WSI) platform API written and maintained by the authors of Iris.\n 
 * Iris comprises multiple modules that can be implemented into whole slide viewer (WSV) solutions in
 * a piece-wise manner. Many of these optional modules perform tasks that could be performed by other 
 * external frameworks, such as performing networking routines, drawing user interfaces, or WSI file decoding.
 * The Iris modules allow for direct integration with the rendering engine and thus are preferred by the authors
 * of Iris but are by no means exclusive; rather we feel it is the programmer's imperative to implement whatever
 * framework they find most effective for the given circumstances.
 * 
 * There are three (3) requirements for the successful implementation of Iris:
 *  1. Add all Iris modules headers to your applications search path for reference during compile time. 
 * *These headers are included, for reference, in the [IrisExamples](https://github.com/ryanlandvater/IrisExamples) for each module and generally comprise 
 * both a **types.hpp** (IrisTypes.hpp), containing the type definitions and API call info_structures, and 
 * the **module.hpp** header (IrisCore.hpp), which contain the methods required to implement the API.* 
 *  2. Link the Iris modules libraries to your application during program linking. *In order to gain access 
 * to the Iris runtime binaries, you must agree to the academic use license. You must contact the [authors](mailto:ryanlandvater@gmail.com) 
 * of Iris for access.*
 *  3. Bind a drawable surface at runtime to allow Iris to configure graphics parameters. 
 * 
 * ## Iris Core Module
 * Iris Core is called from within the Iris namespace and is implemented by constructing an Iris::Viewer instance.
 * 
 * This viewer will operate as the primary API handle for all subsequent core and optional module calls.
 * An Iris::Viewer is created by calling the Iris::create_viewer(const Iris::ViewerCreateInfo&) method.
 * The viewer is created in an inactive state and The viewer is initalized once bound to a drawable surface,
 *  such as an operating system window, via a call to the viewer_bind_external_surface(const ViewerBindExternalSurfaceInfo&) 
 * method. Calls to interface with the engine are made as part of the remaining API methods defined in IrisCore.hpp.
 * 
 * \note The Viewer instance requires initialization by binding it to an operating system (OS) draw surface
 * prior to use.
 * 
 * ## Publications
 * For publications related to Iris and explaining the functionality / architectures of the various WSI modules, please reference the following patents and publications:
 * 
 * Landvater, R. E. (2023).  _Patent No. US-20230334621-A1_. Retrieved from https://patents.google.com/patent/US20230334621A1
 * 
 * Landvater, R., & Balis, U. (2024). As Fast as Glass: A Next Generation Digital Pathology Rendering Engine. United States and Canadian Academy of Pathology 113th Annual Meeting Abstracts.  _Laboratory Investigation_,  _104_(3), 101595. https://doi.org/10.1016/j.labinv.2024.101595
 */
