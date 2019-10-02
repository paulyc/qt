export QT4PREFIX=/opt/qt4

./configure -prefix $QT4PREFIX -sysconfdir /etc/xdg -confirm-license -opensource -release -dbus-linked -openssl-linked -system-sqlite -no-phonon -no-phonon-backend -no-webkit -no-openvg -nomake demos -nomake examples -optimized-qmake && \
make && \
sudo make install
