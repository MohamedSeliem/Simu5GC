//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package simu5gc;

import simu5gc.AMF.Amf;
import simu5gc.CHF.Chf;
import simu5gc.NSSF.Nssf;
import simu5gc.SMF.Smf;
import simu5gc.AUSF.Ausf;
import simu5gc.PCF.Pcf;
import simu5gc.UDM.Udm;
import simu5gc.UDR.Udr;
import simu5gc.BSF.Bsf;
import simu5gc.UPF.Upf;


module Simu5GC
{
    @display("bgb=621,475");
    gates:
        input in[];
        output out[];

    submodules:
        // Control Plane Functions
        amf: Amf {
            @display("p=172,250");
        }
        smf: Smf {
            @display("p=302,250");
        }
        ausf: Ausf {
            @display("p=172,143");
        }
        udm: Udm {
            @display("p=302,143");
        }
        pcf: Pcf {
            @display("p=443,250");
        }
        nssf: Nssf {
            @display("p=172,340");
        }
        bsf: Bsf {
            @display("p=232,182");
        }
        chf: Chf {
            @display("p=443,143");
        }
        udr: Udr {
            @display("p=172,42");
        }

        // User Plane Function
        upf: Upf {
            @display("p=302,340");
        }


    connections:
        // N1, N2, N12, N14
        in[1] --> NInterfaceChannel --> amf.N1In;
        amf.N1Out --> NInterfaceChannel --> out[1];

        in[2] --> NInterfaceChannel --> amf.N2In;
        amf.N2Out --> NInterfaceChannel --> out[2];

        in[12] --> NInterfaceChannel --> amf.N12In;
        amf.N12Out --> NInterfaceChannel --> out[12];

        in[14] --> NInterfaceChannel --> amf.N14In;
        amf.N14Out --> NInterfaceChannel --> out[14];

        // N3, N6, N9
        in[3] --> NInterfaceChannel --> upf.N3In;
        upf.N3Out --> NInterfaceChannel --> out[3];
        
        in[6] --> NInterfaceChannel --> upf.N6In;
        upf.N6Out --> NInterfaceChannel --> out[6];
        
        in[9] --> NInterfaceChannel --> upf.N9In;
        upf.N9Out --> NInterfaceChannel --> out[9];

        // N4
        upf.N4Out --> NInterfaceChannel --> smf.N4In;
        smf.N4Out --> NInterfaceChannel --> upf.N4In;

        // N5
        in[5] --> NInterfaceChannel --> pcf.N5In;
        pcf.N5Out --> NInterfaceChannel --> out[5];

        // N7
        smf.N7Out --> NInterfaceChannel --> pcf.N7In;
        pcf.N7Out --> NInterfaceChannel --> smf.N7In;

        // N8
        amf.N8Out --> NInterfaceChannel --> ausf.N8In;
        ausf.N8Out --> NInterfaceChannel --> amf.N8In;

        // N10
        udm.N10Out --> NInterfaceChannel --> ausf.N10In;
        ausf.N10Out --> NInterfaceChannel --> udm.N10In;

        // N11
        amf.N11Out --> NInterfaceChannel --> smf.N11In;
        smf.N11Out --> NInterfaceChannel --> amf.N11In;

        // N13
        pcf.N13Out --> NInterfaceChannel --> udm.N13In;
        udm.N13Out --> NInterfaceChannel --> pcf.N13In;

        // N15
        pcf.N15Out --> NInterfaceChannel --> chf.in++;
        chf.out++ --> NInterfaceChannel --> pcf.N15In;

        // N17
        amf.N17Out --> NInterfaceChannel --> nssf.N17In;
        nssf.N17Out --> NInterfaceChannel --> amf.N17In;

        // N18
        udm.N18Out --> NInterfaceChannel --> udr.N18In;
        udr.N18Out --> NInterfaceChannel --> udm.N18In;

        // N19
        smf.N19Out --> NInterfaceChannel --> bsf.N19In;
        bsf.N19Out --> NInterfaceChannel --> smf.N19In;

        // N20
        ausf.N20Out --> NInterfaceChannel --> udr.N20In;
        udr.N20Out --> NInterfaceChannel --> ausf.N20In;

        // N21
        amf.N21Out --> NInterfaceChannel --> bsf.N21In;
        bsf.N21Out --> NInterfaceChannel --> amf.N21In;
}
