#pragma once
namespace FFXI {
	namespace Util {
		class md5 {
		public :
			unsigned int hash[4];
			unsigned int bytes[2];
			char context[64];
			void init();
			void update(const void*, unsigned int);
			void final(void*);
			void transform(const void*);
		};
	}
}