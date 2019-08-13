function Component() {
	gui.setSettingsButtonEnabled(false);
	//gui.showSettingsButton(false);
	installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
	component.loaded.connect(this, componentLoaded);
	installer.installationFinished.connect(this, installationFinished);
	installer.finishButtonClicked.connect(this, finishButtonClicked);
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

installationFinished = function() {
	if (installer.isInstaller() && installer.status == QInstaller.Success) {
		installer.addWizardPageItem(component, "LaunchApplicationForm", QInstaller.InstallationFinished);
    }
}

finishButtonClicked = function() {
    if (!component.installed) return;
		
    if (installer.value("os") == "win" && installer.isInstaller() && installer.status == QInstaller.Success) {
		var launchApplication = component.userInterface("LaunchApplicationForm").LaunchApplication.checked;
		if (launchApplication) {
			installer.executeDetached("@TargetDir@\\ScET.exe");
		}
    }
}
