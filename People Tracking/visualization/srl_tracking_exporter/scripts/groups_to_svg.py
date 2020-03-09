#!/usr/bin/env python

"""
Renders groups (tracking_msg/TrackedGroups) and their tracks to an SVG file, (C)2014 Timm Linder
Based upon tracks_to_svg.py, using inheritance
Parameters in addition to those of tracks_to_svg.py:
  None
"""

from tracks_to_svg import *
from tracking_msgs.msg import TrackedGroups, TrackedGroup
import os
import sys
import math
import time
import svgwrite

import roslib
import rospy
roslib.load_manifest('srl_tracking_exporter')

### Database for storing information ###


class GroupDatabase(TrackDatabase):
    def __init__(self):
        super(GroupDatabase, self).__init__()
        self.groups = dict()

### Group ###


class Group(object):
    def __init__(self):
        pass

### Group receiver ###


class GroupReceiver(TrackReceiver):
    def __init__(self):
        super(GroupReceiver, self).__init__()
        self.firstGroupsReceived = False
        self.groupSubscriber = rospy.Subscriber(
            "/perception/tracked_groups", TrackedGroups, self.newGroupsReceived)
        self.groupCycle = -1

    def stop(self):
        super(GroupReceiver, self).stop()
        self.groupSubscriber.unregister()

    def storeGroupId(self, track, step, group_id):
        if not hasattr(track, 'group_ids'):
            track.group_ids = dict()
        track.group_ids[step] = group_id

    def newGroupsReceived(self, trackedGroups):
        with self.mutex:
            if not self.firstGroupsReceived:
                self.firstGroupsReceived = True
                rospy.loginfo("First groups(s) received at %d secs" %
                              int(trackedGroups.header.stamp.secs))

            self.groupCycle += 1
            for trackedGroup in trackedGroups.groups:
                for track_id in trackedGroup.track_ids:
                    track = self.getTrack(track_id)
                    self.storeGroupId(track, self.groupCycle,
                                      trackedGroup.group_id)

### Group renderer ###


class GroupRenderer(TrackRenderer):
    def getTrackColor(self, track_id, trackCycle=0):
        group_id = None
        try:
            track = database.tracks[track_id]
            if hasattr(track, 'group_ids'):
                group_id = track.group_ids[trackCycle]
        except (IndexError, KeyError) as e:
            pass

        if group_id == None or trackCycle == 0:
            return '#888'

        # use group id instead of track id for coloring
        return super(GroupRenderer, self).getTrackColor(group_id)

### Main method ###


def main(argv=None):
    initNode('groups_to_svg')

    database = GroupDatabase()

    groupReceiver = GroupReceiver()
    groupRenderer = GroupRenderer()

    groupReceiver.spin()
    groupRenderer.generateSVG()

    sys.exit(0)


### Globals ###
if __name__ == "__main__":
    main()
