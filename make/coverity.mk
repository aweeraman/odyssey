# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

.PHONY: coverity coverity-submit cov-configure

coverity: cov-configure
ifeq (, $(shell which cov-build))
	$(error cov-build is not available in the PATH)
endif
	cov-build --dir cov-int $(MAKE)
	tar zcvf odyssey-coverity.tar.gz cov-int

coverity-submit: clean coverity
ifndef COVERITY_TOKEN
	$(error COVERITY_TOKEN is not set)
endif
	curl --form token=$(COVERITY_TOKEN) \
	--form email=anuradha@weeraman.com \
	--form file=@odyssey-coverity.tar.gz \
	--form version="$(VERSION)" \
	--form description="An experimental x86 operating system" \
	https://scan.coverity.com/builds?project=minos

cov-configure:
ifeq (, $(shell which cov-configure))
	$(error cov-configure is not available in the PATH)
endif
	cov-configure -co $(CC) -- -ffreestanding
