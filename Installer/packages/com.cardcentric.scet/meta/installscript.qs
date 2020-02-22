function Component() {
	gui.showSettingsButton(false);
	if (installer.isInstaller()) {
		installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
	}
	component.loaded.connect(this, componentLoaded);
}

Component.prototype.createOperations = function() {
    component.createOperations();

    if (installer.value("os") === "win") {
        component.addOperation("RegisterFileType",
                               "sts",
                               "@TargetDir@\\ScET.exe '%1'",
                               "SIM Trace Session",
                               "text/plain",
                               "@TargetDir@\\FileIcon.ico",
                               "ProgId=ScET.sts");
		component.addOperation("CreateShortcut", "@TargetDir@\\ScET.exe", "@StartMenuDir@\\ScET.lnk");
		if (component.userInterface("DesktopShortcutForm").DesktopShortcut.checked) {
			component.addOperation("CreateShortcut", "@TargetDir@\\ScET.exe", "@DesktopDir@\\ScET.lnk");
		}
		if (shouldInstallVCRedist()) {
			QMessageBox.information("vcRedist.install", "Install VS Redistributables", "ScET requires Visual Studio C++ Redistributables. Please follow the installation steps.", QMessageBox.OK);
			component.addOperation("Execute", "@TargetDir@/vc_redist.x64.exe", "/passive", "/norestart");
		}
    }
}

shouldInstallVCRedist = function() {
    var registryVC2017x64 = installer.execute("reg", new Array("QUERY", "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x64", "/v", "Installed"))[0];
    if (!registryVC2017x64) {
        return true;
    } else {
        var bld = installer.execute("reg", new Array("QUERY", "HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes\\x64", "/v", "Bld"))[0];

        var elements = bld.split(" ");

        bld = parseInt(elements[elements.length-1]);
        return bld < 28127;
    }
	
}

componentLoaded = function() {
	if (installer.value("os") === "win" && installer.isInstaller()) {
		installer.addWizardPageItem(component, "DesktopShortcutForm", QInstaller.TargetDirectory);
	}
}
