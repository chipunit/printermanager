//
//  AppDelegate.m
//  Printer Manager
//
//  Created by Developer on 22/09/15.
//  Copyright (c) 2015 Chip Unit. All rights reserved.
//

#import "AppDelegate.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSBundle *bundle = [NSBundle mainBundle];
    statusItem = [[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength];
    statusIcon = [[NSImage alloc] initWithContentsOfFile:[bundle pathForResource:@"appicon" ofType:@"png"]];
    
    [statusItem setToolTip:@"XPrinter manager"];
    [statusItem setImage:statusIcon];
    [statusItem setAlternateImage:statusIcon];
    [statusItem setMenu:statusMenu];
    
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (IBAction) onOpenSite:(id)sender {
    
}

- (IBAction) onQuit:(id)sender {
    
}

@end
