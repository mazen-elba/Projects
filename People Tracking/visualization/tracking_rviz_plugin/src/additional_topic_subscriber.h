#ifndef ADDITIONAL_TOPIC_SUBSCRIBER_H
#define ADDITIONAL_TOPIC_SUBSCRIBER_H

#ifndef Q_MOC_RUN
#include <message_filters/subscriber.h>
#include <tf/message_filter.h>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <iostream>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#endif

#include <rviz/display.h>
#include <rviz/display_context.h>
#include <rviz/frame_manager.h>
#include <rviz/properties/ros_topic_property.h>

using namespace boost;

namespace rviz
{

/** @brief Helper superclass for AdditionalTopicSubscriber, needed because
 * Qt's moc and c++ templates don't work nicely together.  Not
 * intended to be used directly. */
class _AdditionalTopicSubscriber : QObject
{
    Q_OBJECT
public:
    void initialize(Display *display, FrameManager *frameManager)
    {
        QObject::connect(display, SIGNAL(changed()), this, SLOT(displayEnableChanged()));
        QObject::connect(frameManager, SIGNAL(fixedFrameChanged()), this, SLOT(fixedFrameChanged()));
        additional_topic_property_ = new RosTopicProperty("Additional topic", "", "", "", display, SLOT(updateTopic()), this);
    }

protected Q_SLOTS:
    virtual void updateTopic() = 0;
    virtual void displayEnableChanged() = 0;
    virtual void fixedFrameChanged() = 0;

protected:
    RosTopicProperty *additional_topic_property_;
};

/** @brief Display subclass using a tf::MessageFilter, templated on the ROS message type.
 *
 * This class brings together some common things used in many Display
 * types.  It has a tf::MessageFilter to filter incoming messages, and
 * it handles subscribing and unsubscribing when the display is
 * enabled or disabled.  It also has an Ogre::SceneNode which  */
template <class MessageType>
class AdditionalTopicSubscriber : public _AdditionalTopicSubscriber
{
    // No Q_OBJECT macro here, moc does not support Q_OBJECT in a templated class.
public:
    /** @brief Convenience typedef so subclasses don't have to use
     * the long templated class name to refer to their super class. */
    typedef AdditionalTopicSubscriber<MessageType> ATSClass;

    AdditionalTopicSubscriber(const QString &propertyName, Display *display, DisplayContext *context, ros::NodeHandle &update_nh, const function<void(shared_ptr<const MessageType>)> &messageCallback)
        : tf_filter(NULL), m_messagesReceived(0), m_display(display), m_context(context), m_updateNodeHandle(update_nh), m_enabled(false), m_messageCallback(messageCallback)
    {
        _AdditionalTopicSubscriber::initialize(display, context->getFrameManager());

        additional_topic_property_->setName(propertyName);
        QString message_type = QString::fromStdString(ros::message_traits::datatype<MessageType>());
        additional_topic_property_->setMessageType(message_type);
        additional_topic_property_->setDescription(message_type + " topic to subscribe to.");

        tf_filter = new tf::MessageFilter<MessageType>(*m_context->getTFClient(), "map", 10, m_updateNodeHandle);

        tf_filter->connectInput(m_subscriber);
        tf_filter->registerCallback(boost::bind(&AdditionalTopicSubscriber<MessageType>::incomingMessage, this, _1));
        m_context->getFrameManager()->registerFilterForTransformStatusCheck(tf_filter, display);

        setEnabled(m_display->isEnabled());
        updateTopic();
        fixedFrameChanged();
    }

    virtual ~AdditionalTopicSubscriber()
    {
        unsubscribe();
        delete tf_filter;
    }

    virtual void reset()
    {
        tf_filter->clear();
        m_messagesReceived = 0;
    }

    void setEnabled(bool enabled)
    {
        m_enabled = enabled;
        if (enabled)
            subscribe();
    }

protected:
    virtual void updateTopic()
    {
        ROS_DEBUG_STREAM_NAMED("AdditionalTopicSubscriber", "AdditionalTopicSubscriber: Topic was changed to " << additional_topic_property_->getTopicStd());
        unsubscribe();
        reset();
        subscribe();
        m_context->queueRender();
    }

    virtual void displayEnableChanged()
    {
        ROS_DEBUG_STREAM_NAMED("AdditionalTopicSubscriber", "AdditionalTopicSubscriber: Display enabled = " << m_display->getBool());
        setEnabled(m_display->getBool());
    }

    virtual void fixedFrameChanged()
    {
        ROS_DEBUG_STREAM_NAMED("AdditionalTopicSubscriber", "AdditionalTopicSubscriber: Fixed frame has changed for topic " << additional_topic_property_->getTopicStd());
        tf_filter->setTargetFrame(m_context->getFixedFrame().toStdString());
        reset();
    }

    virtual void subscribe()
    {
        if (!m_display->isEnabled())
        {
            return;
        }

        try
        {
            ROS_DEBUG_STREAM_NAMED("AdditionalTopicSubscriber", "AdditionalTopicSubscriber: Subscribing to topic " << additional_topic_property_->getTopicStd());
            m_subscriber.subscribe(m_updateNodeHandle, additional_topic_property_->getTopicStd(), 10);
            m_display->setStatus(StatusProperty::Ok, additional_topic_property_->getName(), "OK");
        }
        catch (ros::Exception &e)
        {
            m_display->setStatus(StatusProperty::Error, additional_topic_property_->getName(), QString("Error subscribing: ") + e.what());
        }
    }

    virtual void unsubscribe()
    {
        m_subscriber.unsubscribe();
    }

    /** @brief Incoming message callback.  Checks if the message pointer
     * is valid, increments m_messagesReceived, then calls
     * processMessage(). */
    void incomingMessage(const typename MessageType::ConstPtr &msg)
    {
        if (!msg)
        {
            return;
        }

        ++m_messagesReceived;
        m_display->setStatus(StatusProperty::Ok, additional_topic_property_->getName(), QString::number(m_messagesReceived) + " messages received");

        // Callback for further processing
        m_messageCallback(msg);
    }

    tf::MessageFilter<MessageType> *tf_filter;

private:
    std::string m_topic;
    bool m_enabled;
    Display *m_display;
    DisplayContext *m_context;
    ros::NodeHandle m_updateNodeHandle;
    message_filters::Subscriber<MessageType> m_subscriber;
    uint32_t m_messagesReceived;

    const function<void(shared_ptr<const MessageType>)> m_messageCallback;
};

} // end namespace rviz

#endif // ADDITIONAL_TOPIC_SUBSCRIBER_H
