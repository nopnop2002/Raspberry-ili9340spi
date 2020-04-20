# Font File   
This tool uses an "IL Font" that has been converted from "IPA fonts" to FONTX format fonts.   
This distribution is distributed under the license of the original distribution IPA font.   
The license of IPA font is in IPA_Font_License_Agreement_v1.0.txt.   
In order to modify and redistribute the IPA font, it is named "IL Font" according to the license terms.   

## FONTX format   
You can add your original font file.   
Please refer [this](http://elm-chan.org/docs/dosv/fontx_e.html) page about FONTX format.   

```
FontxFile yourFont[2];
Fontx_init(yourFont,"./fontx/AlphaNumeric_font","./fontx/JAPANESE_font");
uint8_t ascii[10];
strcpy((char *)ascii, "MyFont");
uint16_t color = RED;
lcdDrawUTF8String(yourFont, xpos, ypos, ascii, color);
```

## FONTX Editor   
[There](http://elm-chan.org/fsw/fontxedit.zip) is FONTX Editor.   
This runs on Windows10.   
Developer page is [here](http://elm-chan.org/fsw_e.html).   

![FontxEditor](https://user-images.githubusercontent.com/6020549/78731275-3b889800-797a-11ea-81ba-096dbf07c4b8.png)


This library uses the following as default fonts:   
- fontx/ILGH16XB.FNT fontx/ILGZ16XB.FNT // 8x16Dot Gothic
- fontx/ILGH24XB.FNT fontx/ILGZ24XB.FNT // 12x24Dot Gothic
- fontx/ILGH32XB.FNT fontx/ILGZ32XB.FNT // 16x32Dot Gothic
- fontx/ILMH16XB.FNT fontx/ILMZ16XF.FNT // 8x16Dot Mincyo
- fontx/ILMH24XB.FNT fontx/ILMZ24XF.FNT // 12x24Dot Mincyo
- fontx/ILMH32XB.FNT fontx/ILMZ32XF.FNT // 16x32Dot Mincyo

Changing this file will change the font.
