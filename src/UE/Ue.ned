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

package simu5gc.UE;

//
// Mobile device that connects to the network
//
simple Ue
{
    parameters:
        @display("i=device/pocketpc");
    gates:
        inout Uu;// gNB	NR Radio Protocol Stack (RRC, PDCP, RLC, MAC, PHY)	Wireless communication between UE and RAN (5G NR)
}
