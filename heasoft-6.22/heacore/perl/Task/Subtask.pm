# $Source$
# $Revision$
# $Date$
#
# $Log$
# Revision 1.1  2004/12/29 21:19:16  rwiegand
# Promoted from UVOT library.
#

package Task::Subtask;
use base qw(Task);


sub _initialize
{
	my ($self) = @_;
	$self->SUPER::_initialize;
	$self->{task} ||= Task->new;
}


sub getTask
{
	return shift->{task};
}


sub temporary
{
	my ($self, $base, %args) = @_;
	return $self->getTask->temporary($base, %args);
}


1;

