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

package simu5gc.GnB;

//
// 5G base station that connects UE to 5GC
//
simple Gnb
{
    parameters:
        @display("i=device/antennatower");
    gates:
        inout Uu;
        inout N1;	// AMF	NAS over NGAP	UE Registration, Authentication, Mobility Management
        inout N2;	// AMF	NGAP (over SCTP)	Control-plane signaling between RAN and Core
        inout N3;	// UPF	GTP-U	User-plane data transfer from UE to Core
        inout N12;	// NGAP	UE context setup, paging, and mobility handling
}
