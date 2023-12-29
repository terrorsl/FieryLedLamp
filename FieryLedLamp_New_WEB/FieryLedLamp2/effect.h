#ifndef EFFECT_FILE
#define EFFECT_FILE

class FieryLedLampEffect
{
public:
	virtual void updateInner() = 0;

	void update();
};

class FieryLedLampEffectWhiteColorStripeRoutine: public FieryLedLampEffect
{
public:
	void updateInner();
};

class FieryLedLampEffectList
{
public:
	void set_effect(uint16_t index);

	void update();
private:
	//FieryLedLampEffect effects
};
#endif