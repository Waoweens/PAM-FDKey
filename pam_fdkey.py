import tomllib
import os
import sys
import dbus
import dbus.service
import dbus.mainloop.glib
from gi.repository import GLib

class FDKeyService(dbus.service.Object):
	def __init__(self, bus_name: str, object_path: str) -> None:
		super().__init__(bus_name, object_path)

	@dbus.service.method('net.meowcorp.FDKey.Authenticate')
	def PubKeyAuth(self, uid: int) -> bool:
		print(f'PubKeyAuth: {uid}')
		return True

def main() -> None:
	# config: dict = readConfig()
	# print(config)

	# dbus main loop
	dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
	bus: dbus.bus.BusConnection = dbus.SystemBus()
	bus_name: dbus.bus.BusName = dbus.service.BusName('net.meowcorp.FDKey', bus=bus)
	object_path: str = '/net/meowcorp/FDKey/Authentication'

	service = FDKeyService(bus_name, object_path)
	loop = GLib.MainLoop()
	loop.run()

def readConfig() -> dict:
	# check for XDG_CONFIG_HOME, default to ~/.config
	if 'XDG_CONFIG_HOME' in os.environ:
		configDir = os.environ['XDG_CONFIG_HOME']
	else:
		configDir = os.path.expanduser('~/.config')

	# make sure user has write access to configDir
	if not os.access(configDir, os.W_OK):
		eprint('Error: no write access to config directory')
		exit(1)

	# check for pam_fdkey.conf
	configFile: str = os.path.join(configDir, 'pam_fdkey.conf')
	if not os.path.isfile(configFile):
		eprint('Error: config file not found')
		exit(1)
	
	# read config file
	with open(configFile, 'rb') as f:
		config: dict = tomllib.load(f)

	return config

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

if __name__ == '__main__':
	main()