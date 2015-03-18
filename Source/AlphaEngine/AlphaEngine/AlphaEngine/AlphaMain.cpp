#include <string>
#include <iostream>
#include "Time\Clock.h"
#include "Time\ClockManager.h"
#include "Time\SystemTime.h"

#include "Memory\MemoryPool.h"
#include "ResourceManager\ResourceManager.h"


using namespace std;
int main(void)
{
	
	Resource* res = new Resource("bob");
	ResourceManager rm;
	rm.Init(50);
	shared_ptr<Resource> pRes(res);
	
	rm.AddResource(pRes);
	rm.Update() ;

}
	
