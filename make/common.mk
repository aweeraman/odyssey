## Usage: make [ARCH=arm|x86] [TARGET]
## Available targets:
##     tags: generates tag file for source code
tags:
	@ctags --exclude=deps/* -R .

##     clean: clears generated files
clean:
	@rm -rf $(ISO) iso odyssey.img odyssey-coverity.tar.gz cov-int
	@rm -f odyssey odyssey.map odyssey.bin
	@find . -not -path "./deps/*" -not -path "./.git/*" \
		\( -name '*.o' -o -name '*.d' -o -name '*.bin' \) \
		-exec rm {} \;
	@rm -f tags
	@$(MAKE) -C tools/ppm-to-array clean 2>/dev/null

##     help: how to build and run
help:
	@sed -n 's/^## //p' Makefile make/*.mk
