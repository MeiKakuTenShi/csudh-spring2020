// Check for the various File API support.
if (window.File && window.FileReader && window.FileList && window.Blob)
{
    // console.log('Great success! All the File APIs are supported.')
} else {
    alert('The File APIs are not fully supported in this browser.');
}

let valid_line_comment = / \/\//;
let invalid_line_comment = /^ *\/(?!=\/|\*)/;

let multi_comment_start = /\/\*/;
let multi_comment_end = /\*\//;

let error_out = document.getElementById('debug');
let result_out = document.getElementById('output');

function handleFileSelect(evt) {
    var files = evt.target.files; // FileList object

    // Loop through the FileList
    for (var i = 0, f; f = files[i]; i++) {
        var reader = new FileReader();

        reader.onload = function(e) {
            let result = ''
            let lines = e.target.result.split('\n');
            // console.log('total # of lines: ' +  lines.length);
            for(var i = 0; i < lines.length;)
            {
                if(valid_line_comment.test(lines[i])) // check for valid line comment
                {
                    var inline = /[\W\w\s]+(?=\/\/)/;
                    result = result + lines[i].match(inline)[0];
                    // console.log('line comment found!\nline #' + (i + 1) + ': ' + lines[i]);
                } else if(multi_comment_start.test(lines[i])) // check for valid multi-line comment; catch invalid comments
                {
                    console.log('start of multi-line comment found!\nline #' + (i + 1) + ': ' + lines[i]);

                    let start = i;
                    // find end of multiline comment
                    for(;start < lines.length;) {
                        if(multi_comment_end.test(lines[start])) {
                            console.log('end of multi-line comment found!\nline #' + (start + 1) + ': ' + lines[start]);
                            i = start;
                            break;
                        } else {
                            start++;
                        }

                        if(start == lines.length) {
                            for(var k = i; k < lines.length; k++) {
                                if(!valid_line_comment.test(lines[k])) {
                                    result = result + lines[k];
                                }
                            }
                            error_out.innerText += 'invalid multiline comment : No End - line '+ (start + 1) + '\n' + lines[i];
                        }
                    }
                    
                } else if(invalid_line_comment.test(lines[i])) // check for invalid line commment
                {
                    error_out.innerText += 'invalid line comment - line ' + (i + 1) + '\n' + lines[i];
                } else // add to result
                {
                    result = result + lines[i];
                }

                i++;
            }

            result_out.innerText += result;
        };

        reader.readAsText(f);
    }

    // files is a FileList of File objects. List some properties.
    var output = [];
    for (var i = 0, f; f = files[i]; i++) {
      output.push('<li><strong>', escape(f.name), '</strong> (', f.type || 'n/a', ') - ',
                  f.size, ' bytes, last modified: ',
                  f.lastModifiedDate ? f.lastModifiedDate.toLocaleDateString() : 'n/a',
                  '</li>');
    }
    document.getElementById('list').innerHTML = '<ul>' + output.join('') + '</ul>';
  }

document.getElementById('files').addEventListener('change', handleFileSelect, false);
