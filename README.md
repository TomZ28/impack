# Image-Packer
A program for hiding files in images.  

## How does it work?
When a device displays an image, it does so up to some end of image/file marker. Anything after is not displayed, so additional data can be stored after the marker. This means that entire files can be stored in an image.

## Usage
### Packing
```bash
$ ./impack <path to image> <path to file>
```
### Extracting
```bash
$ ./impack -e <path to image>
```

## Planned Features
- Packing/extracting multiple files for one image
- Packing/extracting for multiple images at the same time
