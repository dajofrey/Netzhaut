/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

var options = document.getElementsByTagName("OPTION");

for (const option of options) {
    option.addEventListener("click", clickOption()); 
    if (option.selected) {
        option.parentElement.textContent = option.textContent;
        option.parentElement.value = option.value;
    }
}

function clickOption(event) 
{
    var target = event.target;

    if (target.parentElement.tagName == "SELECT") 
    {
        target.parentElement.textContent = target.textContent;
        target.parentElement.value = target.value;
//        for (option of options) {
//            if (option.selected) {
//                option.selected = false;
//            }
//        }
        target.selected = true;
        var selchng = new Event('selectionchange');
        target.parentElement.dispatchEvent(selchng);
    }
}

