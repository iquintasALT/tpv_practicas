#pragma once

class Manager;

class System {
public:
	virtual ~System() {}

	void setMngr(Manager *mngr) {
		manager_ = mngr;
	}

	virtual void init() {}

	virtual void update() {}

protected:
	Manager *manager_;
};

