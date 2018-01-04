#include <stddef.h>
#include <exif_tags.h>


static char *types[0xFFFF+0x01] = {NULL};

char *type(uint16_t addr) {
	return types[addr];
}


void tag_init() {
	types[0x0100] = "ImageWidth";
	types[0x0101] = "ImageLength";
	types[0x0102] = "BitsPerSample";
	types[0x0103] = "Compression";
	types[0x0106] = "PhotometricInterpretation";
	types[0x0112] = "Orientation";
	types[0x0115] = "SamplesPerPixel";
	types[0x011C] = "PlanarConfiguration";
	types[0x0212] = "YCbCrSubSampling";
	types[0x0213] = "YCbCrPositioning";
	types[0x011A] = "XResolution";
	types[0x011B] = "YResolution";
	types[0x0128] = "ResolutionUnit";
	types[0x0132] = "DateTime";
	types[0x010E] = "ImageDescription";
	types[0x010F] = "Make";
	types[0x0110] = "Model";
	types[0x0131] = "Software";
	types[0x013B] = "Artist";
	types[0x8298] = "Copyright";
	types[0x9000] = "ExifVersion";
	types[0xA000] = "FlashpixVersion";
	types[0xA001] = "ColorSpace";
	types[0x9101] = "ComponentsConfiguration";
	types[0x9102] = "CompressedBitsPerPixel";
	types[0xA002] = "PixelXDimension";
	types[0xA003] = "PixelYDimension";
	types[0x927C] = "MakerNote";
	types[0x9286] = "UserComment";
	types[0xA004] = "RelatedSoundFile";
	types[0x9003] = "DateTimeOriginal";
	types[0x9004] = "DateTimeDigitized";
	types[0x9290] = "SubSecTime";
	types[0x9291] = "SubSecTimeOriginal";
	types[0x9292] = "SubSecTimeDigitized";
	types[0xA420] = "ImageUniqueID";
	types[0x829A] = "ExposureTime";
	types[0x829D] = "FNumber";
	types[0x8822] = "ExposureProgram";
	types[0x8824] = "SpectralSensitivity";
	types[0x8827] = "ISOSpeedRatings";
	types[0x8828] = "OECF";
	types[0x9201] = "ShutterSpeedValue";
	types[0x9202] = "ApertureValue";
	types[0x9203] = "BrightnessValue";
	types[0x9204] = "ExposureBiasValue";
	types[0x9205] = "MaxApertureValue";
	types[0x9206] = "SubjectDistance";
	types[0x9207] = "MeteringMode";
	types[0x9208] = "LightSource";
	types[0x9209] = "Flash";
	types[0x920A] = "FocalLength";
	types[0x9214] = "SubjectArea";
	types[0xA20B] = "FlashEnergy";
	types[0xA20C] = "SpatialFrequencyResponse";
	types[0xA20E] = "FocalPlaneXResolution";
	types[0xA20F] = "FocalPlaneYResolution";
	types[0xA210] = "FocalPlaneResolutionUnit";
	types[0xA214] = "SubjectLocation";
	types[0xA215] = "ExposureIndex";
	types[0xA217] = "SensingMethod";
	types[0xA300] = "FileSource";
	types[0xA301] = "SceneType";
	types[0xA302] = "CFAPattern";
	types[0xA401] = "CustomRendered";
	types[0xA402] = "ExposureMode";
	types[0xA403] = "WhiteBalance";
	types[0xA404] = "DigitalZoomRatio";
	types[0xA405] = "FocalLengthIn35mmFilm";
	types[0xA406] = "SceneCaptureType";
	types[0xA407] = "GainControl";
	types[0xA408] = "Contrast";
	types[0xA409] = "Saturation";
	types[0xA40A] = "Sharpness";
	types[0xA40B] = "DeviceSettingDescription";
	types[0xA40C] = "SubjectDistanceRange";
	types[0xA433] = "LensMake";
	types[0xA434] = "LensModel";

	types[0x0201] = "JPEGFileOffset";
	types[0x0202] = "JPEGFileDataSize";

	//types[0x8825] = "GPSInfo";
}

	
