#include"FieryLedLamp.h"

#ifdef USE_MQTT
#define MQTT_STATUS_TOPIC "status"
#define MQTT_COMMAND_TOPIC "/command"
#define MQTT_RESULT_TOPIC "/result"

const char *mqtt_commands[]={
    "power",
    "speed",
    "scale",
    "brigtness",
    "effect"
};
typedef enum
{
    POWER_MQTT,
    SPEED_MQTT,
    SCALE_MQTT,
    BRIGHTNESS_MQTT,
    EFFECT_MQTT
}MQTTCommandType;

#define MQTT_COMMANDS_COUNT sizeof(mqtt_commands)/sizeof(mqtt_commands[0])

void mqttOnConnect(bool sessionPresent)
{
	lamp.setup_mqtt_subscribe();
};
void mqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
	lamp.connect_mqtt();
};
void mqttOnMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
	lamp.update_mqtt(topic, payload);
};

void FieryLedLamp::setup_mqtt()
{
	mqtt.setServer(config.mqtt.server.c_str(), config.mqtt.port);
	mqtt.setCredentials(config.mqtt.login.c_str(), config.mqtt.password.c_str());
	mqtt.setKeepAlive(config.mqtt.keep_alive);
	mqtt.setClientId(config.mqtt.clientid.c_str());

	mqtt.onConnect(mqttOnConnect);
	mqtt.onMessage(mqttOnMessage);
	mqtt.onDisconnect(mqttDisconnect);
};
void FieryLedLamp::setup_mqtt_subscribe()
{
	std::string topic = config.mqtt.clientid + MQTT_COMMAND_TOPIC;
	mqtt.subscribe(topic.c_str(), 0);

	JsonDocument doc;
	doc["power"]=config.power_state;
	doc["effect"]=config.currentEffect;
	
	topic=config.mqtt.clientid + MQTT_RESULT_TOPIC;
	String payload;
	serializeJson(doc,payload);
	mqtt.publish(topic.c_str(), 0, true, payload.c_str());
};

void FieryLedLamp::connect_mqtt()
{
	mqtt.connect();
};

void FieryLedLamp::update_mqtt(const char *topic, const char *payload)
{
    std::string path(topic);
    path.erase(0, config.mqtt.clientid.size());

    if(path==MQTT_COMMAND_TOPIC)
    {
        JsonDocument doc;
        deserializeJson(doc, payload);

        for(unsigned int index=0;index<MQTT_COMMANDS_COUNT;index++)
        {
            if(doc.containsKey(mqtt_commands[index])==false)
                continue;
            
            switch(index)
            {
            case MQTTCommandType::POWER_MQTT:
                power_button(doc[mqtt_commands[index]].as<bool>());
                break;
            case MQTTCommandType::EFFECT_MQTT:
                change_effect(doc[mqtt_commands[index]].as<unsigned short>());
                break;
            }
        }
    }
    //std::string topic = config.mqtt.clientid + MQTT_COMMAND_TOPIC;   
}
#endif