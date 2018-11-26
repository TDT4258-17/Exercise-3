# -*-makefile-*-
#
# Copyright (C) 2018 by <TDT4258-17>
#
# See CREDITS for details about who has contributed to this project.
#
# For further information about the PTXdist project and license conditions
# see the README file.
#

#
# We provide this package
#
PACKAGES-$(PTXCONF_DRIVER_SOUND) += driver-sound

#
# Paths and names and versions
#
DRIVER_SOUND_VERSION	:= 1.0
DRIVER_SOUND		:= driver-sound-$(DRIVER_SOUND_VERSION)
DRIVER_SOUND_URL		:= lndir://$(PTXDIST_WORKSPACE)/local_src/$(DRIVER_SOUND)
DRIVER_SOUND_DIR		:= $(BUILDDIR)/$(DRIVER_SOUND)
DRIVER_SOUND_LICENSE	:= unknown

ifdef PTXCONF_DRIVER_SOUND
$(STATEDIR)/kernel.targetinstall.post: $(STATEDIR)/driver-sound.targetinstall
endif

# ----------------------------------------------------------------------------
# Prepare
# ----------------------------------------------------------------------------

$(STATEDIR)/driver-sound.prepare:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Compile
# ----------------------------------------------------------------------------

$(STATEDIR)/driver-sound.compile:
	@$(call targetinfo)
	$(KERNEL_PATH) $(KERNEL_ENV) $(MAKE) $(KERNEL_MAKEVARS) \
		-C $(KERNEL_DIR) \
		M=$(DRIVER_SOUND_DIR) \
		modules
	@$(call touch)

# ----------------------------------------------------------------------------
# Install
# ----------------------------------------------------------------------------

$(STATEDIR)/driver-sound.install:
	@$(call targetinfo)
	@$(call touch)

# ----------------------------------------------------------------------------
# Target-Install
# ----------------------------------------------------------------------------

$(STATEDIR)/driver-sound.targetinstall:
	@$(call targetinfo)
	$(KERNEL_PATH) $(KERNEL_ENV) $(MAKE) $(KERNEL_MAKEVARS) \
		-C $(KERNEL_DIR) \
		M=$(DRIVER_SOUND_DIR) \
		modules_install
	@$(call touch)

# ----------------------------------------------------------------------------
# Clean
# ----------------------------------------------------------------------------

#$(STATEDIR)/driver-sound.clean:
#	@$(call targetinfo)
#	@$(call clean_pkg, DRIVER_SOUND)

# vim: syntax=make
