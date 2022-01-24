include $(TOPDIR)/rules.mk

PKG_NAME:=sender
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/sender
	DEPENDS:=+libubus +libubox +libblobmsg-json +libiot-c
	CATEGORY:=Extra packages
	TITLE:=A sender program that sends router RAM data to the IBM Watson cloud
endef

define Package/sender/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sender $(1)/usr/bin/sender
	$(INSTALL_BIN) ./files/sender.init $(1)/etc/init.d/sender
endef

$(eval $(call BuildPackage,sender))