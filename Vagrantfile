# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

$script = <<SCRIPT
apt-get update
apt-get install -y xauth clang libgtk-3-dev libgtkspell3-3-dev \
    build-essential
SCRIPT

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
  config.vm.box = "puphpet/ubuntu1404-x64"
  config.ssh.forward_x11 = true
  config.vm.provision :shell, inline: $script
end
