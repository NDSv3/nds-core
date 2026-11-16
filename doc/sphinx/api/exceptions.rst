Exceptions
==========

All NDS exceptions derive from :cpp:class:`nds::NdsError`, which derives from
``std::runtime_error``.

NdsError
--------

.. doxygenclass:: nds::NdsError
   :members:

StateMachineError
-----------------

.. doxygenclass:: nds::StateMachineError
   :members:

StateMachineRollBack
--------------------

.. doxygenclass:: nds::StateMachineRollBack
   :members:

StateMachineNoSuchTransition
----------------------------

.. doxygenclass:: nds::StateMachineNoSuchTransition
   :members:

StateMachineTransitionDenied
----------------------------

.. doxygenclass:: nds::StateMachineTransitionDenied
   :members:

TimeConversionError
-------------------

.. doxygenclass:: nds::TimeConversionError
   :members:

NoPortDefinedError
------------------

.. doxygenclass:: nds::NoPortDefinedError
   :members:

FactoryError
------------

.. doxygenclass:: nds::FactoryError
   :members:

DirectoryNotFoundError
----------------------

.. doxygenclass:: nds::DirectoryNotFoundError
   :members:

DriverNotFound
--------------

.. doxygenclass:: nds::DriverNotFound
   :members:

DriverAlreadyRegistered
-----------------------

.. doxygenclass:: nds::DriverAlreadyRegistered
   :members:

DriverDoesNotExportRegistrationFunctions
----------------------------------------

.. doxygenclass:: nds::DriverDoesNotExportRegistrationFunctions
   :members:

ControlSystemNotFound
---------------------

.. doxygenclass:: nds::ControlSystemNotFound
   :members:

DeviceNotAllocated
------------------

.. doxygenclass:: nds::DeviceNotAllocated
   :members:

DeviceAlreadyCreated
--------------------

.. doxygenclass:: nds::DeviceAlreadyCreated
   :members:

PVAlreadyDeclared
-----------------

.. doxygenclass:: nds::PVAlreadyDeclared
   :members:

MissingInputPV
--------------

.. doxygenclass:: nds::MissingInputPV
   :members:

MissingOutputPV
---------------

.. doxygenclass:: nds::MissingOutputPV
   :members:

MissingDestinationPV
--------------------

.. doxygenclass:: nds::MissingDestinationPV
   :members:

INIParserError
--------------

.. doxygenclass:: nds::INIParserError
   :members:

INIParserMissingSection
-----------------------

.. doxygenclass:: nds::INIParserMissingSection
   :members:

INIParserSyntaxError
--------------------

.. doxygenclass:: nds::INIParserSyntaxError
   :members:

