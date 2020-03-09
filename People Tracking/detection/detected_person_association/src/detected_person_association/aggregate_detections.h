#ifndef _AGGREGATE_DETECTIONS_H
#define _AGGREGATE_DETECTIONS_H

#include <nodelet/nodelet.h>
#include "composite_detected_persons_synchronizer.h"

namespace detected_person_association
{
/// Aggregates multiple tracking_msgs/CompositeDetectedPersons messages into a joint tracking_msgs/CompositeDetectedPerson message without
/// performing any data association. Thus assumes that the sensor fields of the individual input messages are non-overlapping.
/// The input messages must be in the same coordinate frame (header.frame_id).
class AggregateDetectionsNodelet : public nodelet::Nodelet, protected CompositeDetectedPersonsSynchronizer
{
public:
    virtual void onInit();

protected:
    /// Implements method of abstract DetectedPersonsSynchronizer base class
    virtual void onNewInputMessagesReceived(const std::vector<tracking_msgs::CompositeDetectedPersons::ConstPtr> &msgs);

private:
    int m_seq;
};
} // namespace detected_person_association

#endif // _AGGREGATE_DETECTIONS_H
