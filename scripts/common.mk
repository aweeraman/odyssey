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
		\( -name '*.o' -o -name '*.d' -o -name '*.bin' -o -name '*.expand' \) \
		-exec rm {} \;
	@rm -f call_graph.svg
	@rm -f tags
	@$(MAKE) -C tools/ppm-to-array clean 2>/dev/null

##     call_graph: generates visualization for the call graph
call_graph: odyssey
	@echo "Generating call graph..."
ifeq (,$(findstring clang,$(CC)))
	$(shell scripts/call_graph.sh)
else
	@echo "Clang not supported, skipping..."
endif

##     help: how to build and run
help:
	@sed -n 's/^## //p' Makefile make/*.mk
