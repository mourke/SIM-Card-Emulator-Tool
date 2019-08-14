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
    }
}

componentLoaded = function() {
	if (installer.value("os") === "win" && installer.isInstaller()) {
		installer.addWizardPageItem(component, "DesktopShortcutForm", QInstaller.TargetDirectory);
	}
}
