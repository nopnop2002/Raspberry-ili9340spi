JPEG_LIB = jpeg.a
JPEG_DIR = tjpgd3
JPEG_SRCS = $(wildcard $(JPEG_DIR)/*.c)
JPEG_OBJS = $(subst .c,.o,$(JPEG_SRCS))

PNG_LIB = png.a
PNG_DIR = pngle
PNG_SRCS = $(wildcard $(PNG_DIR)/*.c)
PNG_OBJS = $(subst .c,.o,$(PNG_SRCS))

ARFLAG = crv

$(JPEG_LIB) : $(JPEG_OBJS) $(JPEG_SRCS)
	$(AR) $(ARFLAG) $(JPEG_LIB) $(JPEG_OBJS)
	@rm -f $(JPEG_OBJS)
	@echo "$(JPEG_LIB) make success"

$(PNG_LIB) : $(PNG_OBJS) $(PNG_SRCS)
	$(AR) $(ARFLAG) $(PNG_LIB) $(PNG_OBJS)
	@rm -f $(PNG_OBJS)
	@echo "$(PNG_LIB) make success"

lib : $(JPEG_LIB) $(PNG_LIB)
