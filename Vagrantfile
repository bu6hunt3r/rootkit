# -*- mode: ruby -*-
# vi: set ft=ruby :

$provision_script = <<PROVISION
  yum install --quiet -y kernel-devel
  yum install --quiet -y kernel-headers
  yum install --quiet -y gcc gcc-c++ make 
PROVISION

Vagrant.configure("2") do |config|
    config.vm.define "centos-rootkit" do |centos|
      centos.vm.box = "bento/centos-6.10"
      centos.vm.hostname = "rootkit"
      centos.vm.ssh.username = "vagrant"
      centos.vm.ssh.password = "vagrant"
      centos.vm.provision "shell", inline: $provision_script
      centos.ssh.forward_agent = true

      centos.vm.synced_folder "host-share", "/media/host-share", type: "rsync", disabled: false, rsync__verbose: true
      centos.vm.network "public_network"

      centos.vm.provider "virtualbox" do |vb|
        vb.customize ["modifyvm", :id, "--memory", "1024"]
        vb.customize ["modifyvm", :id, "--cpus", "1"]
      end
  end
end


