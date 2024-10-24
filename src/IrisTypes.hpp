/**
 * @file IrisTypes.hpp
 * @author Ryan Landvater
 * @brief Iris Core API Types and Structure Definitions
 * @version 2024.0.3
 * @date 2023-08-26
 * 
 * @copyright Copyright (c) 2023-24
 * Created by Ryan Landvater on 8/26/23.
 * 
 * \note ALL STRUCTURE Variables SHALL have variables named in cammelCase
 * \note ALL CLASSES Variables SHALL have underscores with _cammelCase
 * \note ALL LOCAL variables SHALL use lower-case snake_case
 * 
 */

#ifndef IrisTypes_h
#define IrisTypes_h

namespace Iris {
/**
 * @brief Result flags returned by Iris as part of API calls.
 * 
 */
enum Result : uint32_t {
    IRIS_SUCCESS        = 0,
    IRIS_FAILURE        = 0x00000001,
    IRIS_UNINITIALIZED  = 0x00000002,
};
/**
 * @brief Iris Buffer ownership strength to underlying data. A weak reference
 * only wraps data blocks by reference but has no responsibility over the 
 * creation or freeing of that datablock. Strong references have responsibility
 * over the data backing the buffer and will free the memory on buffer destruction.
 * 
 * \note A weak buffer explicitly is forbidden from resizing the buffer as it *may*
 * invalidate the original pointer.
 * \warning Changing a strong to weak buffer **requires** the calling program
 * take responsibility for the buffer data pointer. It is now that program's
 * responsibility to free that data once finished or a memory leak will ensue.
 * 
 */
enum BufferReferenceStrength {
    /// @brief Only wraps access to the data. No ownership or ability to resize underlying pointer.
    REFERENCE_WEAK      = 0,
    /// @brief Full ownership. Will free data on buffer destruction. Can resize underlying pointer.
    REFERENCE_STRONG    = 1,
};
/**
 * @brief Reference counted data object used to wrap datablocks.
 *
 * It can either strong reference or weak reference the underlying data.
 * The buffer can also shift between weak and strong referrences if chosen;
 * however, this is very dangerous obviously and you need to ensure you
 * are tracking if you have switched from weak to strong or vice versa.
 *
 * \note __INTERNAL__Buffer is an internally defined class. You may optionally
 *  include it in your implementation; however, many class methods are unsafe
 *  as they were created for exclusive use by Iris developers and use of these methods
 *  comes with risk.
 *
 *  \warning Buffer is currently NOT safe for concurrent use on muplitple threads.
 *  This will be fixed in future updates.
 */
using Buffer = std::shared_ptr<class __INTERNAL__Buffer>;
/**
 * @brief Access point to Iris API and controls all elements of Iris viewspace
 * 
 * The viewer the the primary control class that interfaces between external
 * applications and their views, and the iris rendering system. It contains interface
 * capabilities between external controllers, coordinates display presentations between
 * external surfaces, and creates any user interface functionalities. It is created using
 * the Iris::create_viewer(const Iris::ViewerCreateInfo&) method and initialized using
 * the Iris::viewer_bind_external_surface(const Iris::ViewerBindExternalSurfaceInfo&) method.
 * \sa ViewerCreateInfo and ViewerBindExternalSurfaceInfo
 * 
 * \note __INTERNAL__Viewer is an internally defined class and not externally exposed.
 */
using Viewer = std::shared_ptr <class __INTERNAL__Viewer>;
/**
 * @brief Handle to Slide File and Slide Loading Routines (Slide Loader)
 * 
 * The Slide object represents a mapped slide file and high-performance loading
 * routines to bring slide data into RAM with limited overhead
 */
using Slide  = std::shared_ptr<class  __INTERNAL__Slide>;

/**
 * @brief Defines necesary runtime parameters for starting the Iris rendering engine.
 * 
 * These runtime parameters will be forwarded to the GPU for certain task tracking
 * and the application bundle path (term from Apple's OS) is important for loading
 * referenced / included runtime files.
 * 
 * Additional runtime parameters will be added as needed in the future.
 */
struct ViewerCreateInfo {
    /// @brief Informs the rendering engine of the calling application's name
    const char*         ApplicationName;
    /// @brief Informs the engine of the calling application version
    uint32_t            ApplicationVersion;
    /// @brief provides the executable location. This is needed for runtime
    /// loading of application files such as UI markup files and shader code.
    const char*         ApplicationBundlePath;
};
/**
 * @brief  System specific binding information to configure Iris' rendering engine
 * for the given operating system draw surface. 
 * 
 * Compilier macros control the structure's definition and backend implementation
 * and thus define the nature of the OS draw surface handles.
 *  - Windows: requires HINSTANCE and HWND handles from the WIN32 API
 *  - Apple: macOS and iOS require a __bridge pointer to a CAMetalLayer
 * 
 */
struct ViewerBindExternalSurfaceInfo {
    const Viewer        viewer      = nullptr;  
#if defined _WIN32
    HINSTANCE           instance    = NULL;    
    HWND                window      = NULL;    
#elif defined __APPLE__
    const void*         layer       = nullptr; 
#endif
};
/**
 * @brief  Information to translate the rendered scope view as a fraction of the active
 * view space with direction given by the sign.
 * 
 * An x translation value of 0.5 will shift the view to the right by half of the current
 * view sapce while -1.0 will shift the scope view to the left by an entire screen.
 * 
 */
struct ViewerTranslateScope {
    /// @brief Fraction of *horizontal* viewspace to translate [-1,1](-left, +right)
    float               x_translate = 0.f;
    /// @brief Fraction of *vertical* viewspace to translate [-1,1](-left, +right)
    float               y_translate = 0.f;
    /// @brief Horizontal translation velocity (suggested [0,2])
    float               x_velocity  = 0.f;
    /// @brief Velocity of translation (suggested [0,2]) in the horizontal
    float               y_velocity  = 0.f;
};
/**
 * @brief Information to change the zoom objective.
 * 
 * A positive zoom increment will increase the scope view 
 * zoom while a negative increment will decrease the current zoom.
 * The zoom origin (x_location and y_location) defines the region
 * around which to zoom. This is best set as either the cursor location
 * or view center (0.5, 0.5).
 * 
 */
struct ViewerZoomScope {
    /// @brief Fraction of current zoom amount by which to increase or decrease
    float               increment   = 0.f;
    /// @brief Horizontal location of zoom origin (towards or way from this point)
    float               x_location  = 0.5f;
    /// @brief Vertical location of zoom origin
    float               y_location  = 0.5f;
};
/**
 * @brief Defines the image encoding format for an image annotation.
 * 
 */
enum AnnotationFormat {
    ANNOTATION_FORMAT_UNDEFINED     = -1,
    ANNOTATION_FORMAT_PNG,
    ANNOTATION_FORMAT_JPEG,
};
/** \def SlideAnnotation::format
 * The AnnotationFormat of the image data to be rendered
 */
/**
 * @brief Structure defining requirements to create an image-based
 * slide annotation.
 * 
 * The required information includes the location of the slide annotation
 * on the slide and the size of the annotation. The offset locations are 
 * fractions of the current view window (for example an annotation that
 * starts in the middle of the current view would have an offset of 0.5)
 * The engine will immediately begin rendering the image on top of the 
 * rendered slide layers.
 */
struct SlideAnnotation {
    /// @brief AnnotationFormat of the image data to be rendered
    AnnotationFormat    format      = ANNOTATION_FORMAT_UNDEFINED;
    /// @brief x-offset of the current scope view window where the image starts [0,1.f]
    float               x_offset    = 0.f;
    /// @brief y-offset of the current scope view window where the image starts [0,1.f]
    float               y_offset    = 0.f;
    /// @brief Number of horizontal (x) pixels in the image annotation
    float               width       = 0.f;
    /// @brief Number of vertical (y) pixels in the image annotation
    float               height      = 0.f;
    /// @brief Encoded pixel data that comprises the image, width wide and hight tall
    Buffer              data;
};
/**
 * @brief  Slide objective layer extent detailing the extent of each objective layer in
 * the number of 256 pixel tiles in each dimension.  
 * 
 * The relative scale (zoom amount) as well as how downsampled the layer is relative to 
 * the highest zoom layer (the reciprocal of the scale).
 */
struct LayerExtent {
    /// @brief Number of horizontal 256 pixel tiles
    uint32_t            xTiles      = 1; 
    /// @brief Number of vertical 256 pixel tiles
    uint32_t            yTiles      = 1; 
    /// @brief How magnified this level is relative to the unmagnified size of the tissue
    float               scale       = 1.f;
    /// @brief Reciprocal scale factor relative to the most zoomed level (for OpenSlide compatibility)
    float               downsample  = 1.f;
};
using LayerExtents = std::vector<LayerExtent>;
/**
 * @brief The extent, in pixels, of a whole side image file. 
 * 
 * These are in terms of the initial layer presented (most zoomed out layer).
 */
struct Extent {
    /// @brief Top (lowest power) layer width extent in screen pixels
    uint32_t            width       = 1; 
    /// @brief Top (lowest power) layer height in screen pixels
    uint32_t            height      = 1; 
    /// @brief Slide objective layer extent list
    LayerExtents        layers; 
};
/**
 * @brief Image channel byte order in little-endian format
 * 
 * Assign this format to match the image source bits per
 * pixel and bit-ordering. 
 */
enum Format {
    /// @brief Invalid format indicating a format was not selected
    FORMAT_UNDEFINED,
    /// @brief 8-bit blue, 8-bit green, 8-bit red, no alpha
    FORMAT_B8G8R8,
    /// @brief 8-bit red, 8-bit green, 8-bit blue, no alpha
    FORMAT_R8G8B8,
    /// @brief 8-bit blue, 8-bit green, 8-bit red, 8-bit alpha
    FORMAT_B8G8R8A8,
    /// @brief 8-bit red, 8-bit green, 8-bit blue, 8-bit alpha
    FORMAT_R8G8B8A8,
};
/**
 * @brief Information to open a slide file located on a local volume.
 * 
 * Provide the file location and the 
 * 
 */
struct LocalSlideOpenInfo {
    const char*         filePath;
    /**
     * @brief Local slide file encoding type
     * 
     * This informs the Iris::Slide object how it should
     * attempt to open and map the slide file. If unknown,
     * it will attempt both encoding sequences. OpenSlide
     * is not supported on all platforms (iOS for example).
     * 
     */
    enum : uint8_t {
        SLIDE_TYPE_UNKNOWN,         //< Unknown file encoding
        SLIDE_TYPE_IRIS,            //< Iris Codec File
        SLIDE_TYPE_OPENSLIDE,       //< Vendor specific file (ex SVS)
    }                   type        = SLIDE_TYPE_UNKNOWN;
    
};
/**
 * @brief Information needed to open a server-hosted slide file.
 * 
 * This requires use of the Iris Networking module.
 * 
 */
struct NetworkSlideOpenInfo {
    const char*         slideID;
};
/**
 * @brief Parameters required to create an Iris::Slide WSI file handle.
 * 
 * This parameter structure is a wrapped union of either
 * a local slide file open information struct or a network hosted
 * slide file open information struct. To allow the system to access
 * the correct union member, a type enumeration must also be defined
 * prior to passing this information stucture to the calling method
 * Iris::create_slide(const SlideOpenInfo&) or
 * Iris::viewer_open_slide(const Viewer&, const SlideOpenInfo&)
 * 
 * Optional parameters that can be used to optimize performance
 * characteristics are also included in the struct. Some are used interally
 * by the Iris rendering engine, and these are invoked when using 
 * Iris::viewer_open_slide(const Viewer&, const SlideOpenInfo&)
 * rather than the more generic Iris::create_slide(const SlideOpenInfo&),
 * so the former should be preferred when available.
 * 
 */
struct SlideOpenInfo {
    enum : uint8_t {
        SLIDE_OPEN_UNDEFINED,           // Default / invalid file
        SLIDE_OPEN_LOCAL,               // Locally accessible / Mapped File
        SLIDE_OPEN_NETWORK,             // Sever hosted slide file
    }                   type            = SLIDE_OPEN_UNDEFINED;
    union {
    /**
     * @brief Information for opening a file on the local machine
     */
    LocalSlideOpenInfo   local;
    /**
     * @brief Information for opening a network hosted file
     */
    NetworkSlideOpenInfo network;
    };
    // ~~~~~~~~~~~~~ OPTIONAL FEATURES ~~~~~~~~~~~~~~~ //
    /**
     * @brief This is the default slide cache capacity
     *
     * The capacity determines the number of allowed cached tiles.
     * This is the primary way in which Iris consumes RAM.
     * Greater values cache more in-memory decompressed tile data
     * for greater performance. Less require more pulls from
     * disk (which is slower)
     * The default 1000 for RGBA images consumes 2 GB of RAM.
     */
    size_t               capacity       = 1000;
};
using LambdaPtr         = std::function<void()>;
using LambdaPtrs        = std::vector<LambdaPtr>;
} // END IRIS NAMESPACE

#endif /* IrisTypes_h */
