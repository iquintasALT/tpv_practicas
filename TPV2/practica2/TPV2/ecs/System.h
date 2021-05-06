#pragma once

class Manager;
struct Message;

class System {
public:
	virtual ~System() {};

	void setMngr(Manager* mngr) { manager_ = mngr; };

	virtual void init() {};

	virtual void update() {};

	virtual void render() {};

	virtual void receive(const Message& msg) {};

protected:
	Manager* manager_;
};

