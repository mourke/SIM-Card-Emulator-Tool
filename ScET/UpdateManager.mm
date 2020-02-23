#include "UpdateManager.h"
#include <Sparkle/Sparkle.h>
#include <Foundation/Foundation.h>

class UpdateManager::UpdateManagerPrivate {
	public:
        SUUpdater *updater;
};

UpdateManager::UpdateManager() {
    p = new UpdateManagerPrivate();

    p->updater = [[SUUpdater sharedUpdater] retain];
    [p->updater setAutomaticallyChecksForUpdates:NO];
    [p->updater setAutomaticallyDownloadsUpdates:NO];

    NSURL *url = [NSURL URLWithString:[NSString stringWithCString:"https://mourke.github.io/macOS/appcast.xml" encoding:NSUTF8StringEncoding]];
    [p->updater setFeedURL:url];
}

long long int UpdateManager::daysSinceLastVersionCheckDate() const {
    NSDate *date = [p->updater lastUpdateCheckDate];
    NSDateComponents *components = [[NSCalendar currentCalendar] components:NSCalendarUnitDay fromDate:date toDate:[NSDate date] options:0];
    NSInteger day = components.day;
    return static_cast<long long int>(day);
}

void UpdateManager::checkForUpdates(CheckFrequency frequency) {
    if (frequency == CheckFrequency::Immediately) {
        [p->updater checkForUpdates:nil]; // user has requested a check, show them UI
    } else if (static_cast<int>(frequency) <= daysSinceLastVersionCheckDate()) {
        [p->updater checkForUpdatesInBackground]; // silent check
    } else {
        // do nothing.
    }
}

UpdateManager::~UpdateManager() {
    [p->updater release];
    delete p;
}



