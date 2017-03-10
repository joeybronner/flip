#!/bin/bash
for (( ; ; ))
do
    focused_window_id=$(xdotool getwindowfocus)
    active_window_id=$(xdotool getactivewindow)
    active_window_pid=$(xdotool getwindowpid "$active_window_id")
    echo "infinite loops [ hit CTRL+C to stop]"
    sleep 1;
done
