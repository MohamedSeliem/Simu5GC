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

package simu5gc.UPF;

//
// Routes user traffic between UE and external networks
//
simple Upf
{
    parameters:
        @display("i=abstract/server");
    gates:
        input N3In;
        output N3Out;

		input N4In;
        output N4Out;
        
        input N6In;
        output N6Out;
        
        input N9In;
        output N9Out;
}
