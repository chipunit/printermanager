//
//  AppDelegate.h
//  Printer Manager
//
//  Created by Developer on 22/09/15.
//  Copyright (c) 2015 Chip Unit. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate> {

    IBOutlet NSMenu *statusMenu;
    NSStatusItem *statusItem;
    NSImage *statusIcon;
    
}

- (IBAction) onOpenSite:(id)sender;
- (IBAction) onQuit:(id)sender;

@end

