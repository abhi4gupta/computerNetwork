"""
 
    [ sw1 ]================================.
      ,---'       |           |           |
    [ sw1_c1 ]=. [ sw1_c2 ]=. [ sw1_c3 ]=. [ sw1_c4 ]=.
    [ PC1c1 ]-|  [ PC1c2 ]-|  [ PC1c3 ]-|  [ PC1c4 ]-|
    [ PC2c1 ]-|  [ PC2c2 ]-|  [ PC2c3 ]-|  [ PC2c4 ]-|
    [ PC3c1 ]-|  [ PC3c2 ]-|  [ PC3c3 ]-|  [ PC3c4 ]-|
    [ PC4c1 ]-'  [ PC4c2 ]-'  [ PC4c3 ]-'  [ PC4c4 ]-'
"""
 
from mininet.topo import Topo
from mininet.util import irange
 
class topology( Topo ):
    "Datacenter topology with 4 clients host per stacks, 4 stackses, and a root switch"
 
    def build( self ):
        self.stackses = []
        mainSwitch = self.addSwitch( 'sw1' )
        for i in irange( 1, 4 ):
            stacks = self.makeRakes( i )
            self.stackses.append( stacks )
            for switch in stacks:
                self.addLink( mainSwitch, switch )
 
    def makeRakes( self, loc ):
        "Build a stacks of hosts with a top-of-stacks switch"
 
        dpid = ( loc * 16 ) + 1
        switch = self.addSwitch( 'sw1_c%s' % loc, dpid='%x' % dpid )
 
        for n in irange( 1, 4 ):
            host = self.addHost( 'PC%sc%s' % ( n, loc ) )
            self.addLink( switch, host )
 
        # Return list of top-of-stacks switches for this stacks
        return [switch]
 
# Allows the file to be imported using `mn --custom <filename> --topo dcbasic`
topos = {
    'data': topology
}
