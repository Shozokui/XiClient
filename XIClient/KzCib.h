#pragma once
namespace FFXI {
	namespace CYy {
		class KzCib {
		public:
            KzCib();
            unsigned char legs4;
            unsigned char material_foot;
            unsigned char size_foot;
            unsigned char amot_wep0;
            unsigned char inout_wep0;
            unsigned char is_shield;
            unsigned char constrain_no;
            unsigned char inout_wep1;
            unsigned char atksch_wep;
            unsigned char waist_type;
            unsigned char scale[4];
            unsigned char field_E;
            unsigned char field_F;
		};
	}
}