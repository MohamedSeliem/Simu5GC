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

package simu5gc.PCF;

//
// Manages network policies, QoS, and traffic control
//
simple Pcf
{
    parameters:
        @display("i=abstract/server");
    gates:
        input N5In;
        output N5Out;
        
        input N7In;
        output N7Out;
        
        input N13In;
        output N13Out;
        
        input N15In;
        output N15Out;
}
