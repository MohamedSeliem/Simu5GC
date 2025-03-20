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

package simu5gc.AMF;

//
// Handles UE registration, mobility, NAS signaling
//
simple Amf
{
    parameters:
        @display("i=abstract/server,red;is=n");
    gates:
        input N1In;
        output N1Out;

        input N2In;
        output N2Out;

        input N8In;
        output N8Out;

        input N11In;
        output N11Out;

        input N12In;
        output N12Out;

        input N14In;
        output N14Out;

        input N17In;
        output N17Out;

        input N21In;
        output N21Out;


}
