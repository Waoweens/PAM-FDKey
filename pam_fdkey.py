import tomllib
import os

def main() -> None:
	readConfig()

def readConfig() -> dict:
	# check for XDG_CONFIG_HOME, default to ~/.config
	if 'XDG_CONFIG_HOME' in os.environ:
		configDir = os.environ['XDG_CONFIG_HOME']
	else:
		configDir = os.path.expanduser('~/.config')

	# make sure user has write access to configDir
	if not os.access(configDir, os.W_OK):
		print('Error: no write access to config directory')
		exit(1)

	# check for pam_fdkey.toml
	configFile: str = os.path.join(configDir, 'pam_fdkey.toml')
	if not os.path.isfile(configFile):
		print('Error: config file not found')
		exit(1)
	
	# read config file
	with open(configFile, 'r') as f:
		config: dict = tomllib.load(f)

	print(config)

	return config

if __name__ == '__main__':
	main()