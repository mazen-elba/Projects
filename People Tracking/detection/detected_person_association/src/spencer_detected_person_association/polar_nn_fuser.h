#ifndef _EUCLIDEAN_NN_FUSER_H
#define _EUCLIDEAN_NN_FUSER_H

#include <nodelet/nodelet.h>
#include "nn_fuser.h"

namespace detected_person_association
{
/// Fuses multiple tracking_msgs/CompositeDetectedPersons messages into a joint tracking_msgs/CompositeDetectedPerson message
/// by associating detections received on different topics based upon their distance to each other, using thresholding on the polar coordinates
/// (useful if distance estimates are not very reliable).
/// The input messages must be in the same coordinate frame (header.frame_id), which can be ensured via ConvertToCompositeDetectionsNodelet.
class PolarNNFuserNodelet : public NearestNeighborFuserNodelet
{
protected:
    /// Compute the distance between a pair of composite detections using polar coordinates. If outside of gating zone, returns infinity.
    virtual float computeDistance(const tracking_msgs::CompositeDetectedPerson &d1, const tracking_msgs::CompositeDetectedPerson &d2);

    /// Fuse the poses of two composite detections
    virtual void fusePoses(const tracking_msgs::CompositeDetectedPerson &d1, const tracking_msgs::CompositeDetectedPerson &d2, geometry_msgs::PoseWithCovariance &fusedPose);
};
} // namespace detected_person_association

#endif // _EUCLIDEAN_NN_FUSER_H
