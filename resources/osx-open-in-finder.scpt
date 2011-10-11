on run argv
    set p to item 1 of argv
    set f to POSIX file p
    tell application "Finder"
        activate
        reveal f
    end tell
    return
end run
