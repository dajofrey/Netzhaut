/**
 * Netzhaut - Web Browser Engine 
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

{
    var x = 0;
    var y = 0;
    var z = 0;
    document.addEventListener("wheel", handleWheel());
    function handleWheel(event) {
        var delta = event.deltaX * 30;
        if (delta > 0 && delta + x + document.documentElement.clientWidth <= document.documentElement.scrollWidth) {
            window.scrollBy(delta, 0);
            x += delta;
        }
        if (delta < 0 && delta + x + document.documentElement.clientWidth >= document.documentElement.clientWidth) {
            window.scrollBy(delta, 0);
            x += delta;
        }
    }
}

