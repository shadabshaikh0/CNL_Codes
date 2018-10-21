import java.util.*;
import java.io.*;
import java.net.*;

class subnet {
    Scanner input = new Scanner(System.in);

    static String appendzeroes(String parameterstring) {
        String temporarystring = new String("00000000");
        return temporarystring.substring(parameterstring.length()) + parameterstring;
    }

    void subnetmask() throws Exception {
        System.out.println("Enter IP address Ex.(192.168.1.0)");
        String ipaddress = input.next();

        String split_ip[] = ipaddress.split("\\.");
        String binaryipaddress = "";
        String temporarybinaryipaddress = "";

        // converting IP to Binary
        String binary_split_ip[] = new String[4];
        for (int i = 0; i < 4; i++) {
            binary_split_ip[i] = appendzeroes(Integer.toBinaryString(Integer.parseInt(split_ip[i])));
            temporarybinaryipaddress = temporarybinaryipaddress + binary_split_ip[i] + ".";
            binaryipaddress += binary_split_ip[i];
        }

        System.out.println("IP ADDRESS (Binary Form): " + temporarybinaryipaddress);

        System.out.println("Enter No of address you want in each subnet");
        int noofaddress = input.nextInt();

        int bits = (int) Math.ceil(Math.log(noofaddress) / Math.log(2));
        System.out.println("Number of bits : " + bits);

        int noofmaskbits = 32 - bits;
        int noofaddresses_subnet = (int) Math.pow(2, bits);

        System.out.println("Number of mask bits : " + noofmaskbits);

        int answer_from_modby8 = noofmaskbits % 8;

        int address = 0;
        for (int i = 0, m = 7; i < answer_from_modby8; i++, m--) {
            address = address + (int) Math.pow(2, m);
        }

        // Default Subnet Mask
        if (noofmaskbits >= 0 && noofmaskbits <= 8) {
            System.out.println("Subnet mask : = " + address + "0.0.0");
        } else if (noofmaskbits >= 8 && noofmaskbits <= 16) {
            System.out.println("Subnet mask : = 255." + address + ".0.0");
        } else if (noofmaskbits >= 16 && noofmaskbits <= 24) {
            System.out.println("Subnet mask : = 255.255." + address + ".0");
            // First Address Calculation
            int firstbinaryipaddress[] = new int[32];
            for (int i = 0; i < 32; i++) {
                firstbinaryipaddress[i] = binaryipaddress.charAt(i) - 48;
            }

            for (int i = 31; i > 31 - bits; i--) {
                firstbinaryipaddress[i] &= 0;
            }

            String firstipaddress[] = { "", "", "", "" };
            for (int i = 0; i < 32; i++) {
                firstipaddress[i / 8] = new String(firstipaddress[i / 8] + firstbinaryipaddress[i]);
            }

            int IPADDRESSFIRST[] = new int[4];
            System.out.print("Group 1 : \n First Address :");
            for (int i = 0; i < 4; i++) {
                if (i == 3) {
                    System.out.print(IPADDRESSFIRST[i] = 0);
                } else {
                    System.out.print(IPADDRESSFIRST[i] = Integer.parseInt(firstipaddress[i], 2));
                }
                if (i != 3)
                    System.out.print(".");
            }
            System.out.println();
            // Last Address Calculation
            int lastbinaryipaddress[] = new int[32];
            for (int i = 0; i < 32; i++) {
                lastbinaryipaddress[i] = binaryipaddress.charAt(i) - 48;
            }

            for (int i = 31; i > 31 - bits; i--) {
                lastbinaryipaddress[i] |= 1;
            }

            String lastipaddress[] = { "", "", "", "" };
            for (int i = 0; i < 32; i++) {
                lastipaddress[i / 8] = new String(lastipaddress[i / 8] + lastbinaryipaddress[i]);
            }

            int IPADDRESSLAST[] = new int[4];
            System.out.print("\n Last Address :");
            for (int i = 0; i < 4; i++) {
                System.out.print(IPADDRESSLAST[i] = Integer.parseInt(lastipaddress[i], 2));
                if (i != 3)
                    System.out.print(".");
            }
            System.out.println();
            
        } else if (noofmaskbits >= 24 && noofmaskbits <= 32) {
            System.out.println("Subnet mask : = 255.255.255." + address);

            System.out.println("------------------------------------------");

            // First Address Calculation
            int firstbinaryipaddress[] = new int[32];
            for (int i = 0; i < 32; i++) {
                firstbinaryipaddress[i] = binaryipaddress.charAt(i) - 48;
            }

            for (int i = 31; i > 31 - bits; i--) {
                firstbinaryipaddress[i] &= 0;
            }

            String firstipaddress[] = { "", "", "", "" };
            for (int i = 0; i < 32; i++) {
                firstipaddress[i / 8] = new String(firstipaddress[i / 8] + firstbinaryipaddress[i]);
            }

            int IPADDRESSFIRST[] = new int[4];
            System.out.print("Group 1 : \n First Address :");
            for (int i = 0; i < 4; i++) {
                if (i == 3) {
                    System.out.print(IPADDRESSFIRST[i] = 0);
                } else {
                    System.out.print(IPADDRESSFIRST[i] = Integer.parseInt(firstipaddress[i], 2));
                }
                if (i != 3)
                    System.out.print(".");
            }
            System.out.println();
            // Last Address Calculation
            int lastbinaryipaddress[] = new int[32];
            for (int i = 0; i < 32; i++) {
                lastbinaryipaddress[i] = binaryipaddress.charAt(i) - 48;
            }

            for (int i = 31; i > 31 - bits; i--) {
                lastbinaryipaddress[i] |= 1;
            }

            String lastipaddress[] = { "", "", "", "" };
            for (int i = 0; i < 32; i++) {
                lastipaddress[i / 8] = new String(lastipaddress[i / 8] + lastbinaryipaddress[i]);
            }

            int IPADDRESSLAST[] = new int[4];
            System.out.print("\n Last Address :");
            for (int i = 0; i < 4; i++) {
                if (i == 3) {
                    System.out.print(IPADDRESSLAST[i] = IPADDRESSFIRST[i] + noofaddresses_subnet - 1);
                } else {
                    System.out.print(IPADDRESSLAST[i] = Integer.parseInt(lastipaddress[i], 2));
                }
                if (i != 3)
                    System.out.print(".");
            }
            System.out.println();
            System.out.println("------------------------------------------");

            int numberofsubnets = 256 / noofaddresses_subnet;

            for (int j = 1; j < numberofsubnets; j++) {

                System.out.print("Group " + (j + 1) + ": \n First Address :");
                for (int i = 0; i < 4; i++) {
                    if (i < 3)
                        System.out.print(IPADDRESSFIRST[i] + ".");
                    else
                        System.out.println(IPADDRESSFIRST[i] += noofaddresses_subnet);
                }

                System.out.print(" \n Last Address :");
                for (int i = 0; i < 4; i++) {
                    if (i < 3)
                        System.out.print(IPADDRESSLAST[i] + ".");
                    else
                        System.out.println(IPADDRESSLAST[i] += noofaddresses_subnet);
                }

            }
        }

    }

    public static void main(String[] args) throws Exception {
        subnet sub = new subnet();
        sub.subnetmask();
    }
}
