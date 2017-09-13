# $Source$
# $Revision$
# $Date$
#
#	This Task subclass provides support for Getopt::Long option parsing
#
# $Log$
# Revision 1.1  2002/09/30 18:34:31  wiegand
# Initial revision
#

package Task::Getopt;

use strict;

use base qw(Task);

use Task qw(:codes);
use Getopt::Long;


sub parseOptions
{
	my ($self, %args) = @_;

	local(@main::ARGV) = @{ $args{args} };

	if ($self->isValid and $args{internal}) {
		foreach my $p (keys(%{ $args{internal} })) {
			$self->set($p, $args{internal}{$p});
		}
	}

	if ($self->isValid and $args{spec}) {
		if (not Getopt::Long::GetOptions($self->args, @{ $args{spec} })) {
			$self->fatal(BAD_INPUT, "unable to parse options: $!");
		}
	}

	if ($self->isValid and $args{default}) {
		foreach my $p (keys(%{ $args{default} })) {
			my $v = $self->get($p);
			if (not defined($v)) {
				$self->set($p, $args{default}{$p});
			}
		}
	}
}


1;

