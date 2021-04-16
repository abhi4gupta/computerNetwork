#!/usr/bin/python

"""
A simple minimal topology script for Mininet.

Based in part on examples in the [Introduction to Mininet] page on the Mininet's
project wiki.

[Introduction to Mininet]: https://github.com/mininet/mininet/wiki/Introduction-to-Mininet#apilevels

"""

from mininet.cli import CLI
from mininet.log import setLogLevel
from mininet.net import Mininet
from mininet.topo import Topo
from mininet.node import RemoteController, OVSSwitch
from mininet.util import irange


class toplogy( Topo ):
    "Datacenter topology with 4 hosts per rack, 4 racks, and a root switch"

    def build( self ):
        self.racks = []
        mainSwitch= self.addSwitch( 's1' )
        for i in irange( 1, 4 ):
            rack = self.makeStack( i )
            self.racks.append( rack )
            for switch in rack:
                self.addLink( mainSwitch, switch )

    def makeStack( self, loc ):
        "Build a rack of hosts with a top-of-rack switch"

        dpid = ( loc * 16 ) + 1
        switch = self.addSwitch( 'Sw1_c%s' % loc, dpid='%x' % dpid )

        for n in irange( 1, 4 ):
            num = (loc-1)*4 + n;
            host = self.addHost( 'PC%sc%s' % ( n, loc ) , ip = '10.0.0.%s' % num , mac = '00:00:00:00:00:%s%s' % (n,loc))
            self.addLink( switch, host )

        # Return list of top-of-rack switches for this rack
        return [switch]
        

def rminiTop():
    "Bootstrap a Mininet network using the Minimal Topology"

    # Create an instance of our topology
    topo = toplogy()

    # Create a network based on the topology using OVS and controlled by
    # a remote controller.
    net = Mininet(
        topo=topo,
        controller=lambda name: RemoteController( name, ip='127.0.0.1' ),
        switch=OVSSwitch,
        autoSetMacs=True )

    # Actually start the network
    net.start()

    # Drop the user in to a CLI so user can run commands.
    CLI( net )

    # After the user exits the CLI, shutdown the network.
    net.stop()

if __name__ == '__main__':
    # This runs if this file is executed directly
    setLogLevel( 'info' )
    rminiTop()

# Allows the file to be imported using `mn --custom <filename> --topo minimal`
topos = {
    'data': toplogy
}

