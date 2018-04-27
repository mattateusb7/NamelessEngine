#pragma once
#include "NamelessCore.h"
namespace _NL {
	namespace Component {
		template<typename ScriptType = void>
		class CppScript : public _NL::Core::Component
		{
		public:
			
			CppScript() {
				
			}

			~CppScript() {
				delete cpps;
			}

			virtual CppScript* clone() const {
				return new CppScript(static_cast<CppScript const&>(*this));
			}

			ScriptType* getScript() {
				return cpps;
			}

			char* ClassName() const override{
				return "_NL::Component::CppScript";
			};

		private:
			ScriptType* cpps = new ScriptType();
		};
	}
}

