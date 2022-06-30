
  var Module = typeof Module !== 'undefined' ? Module : {};
  
  if (!Module.expectedDataFileDownloads) {
    Module.expectedDataFileDownloads = 0;
  }
  Module.expectedDataFileDownloads++;
  (function() {
   var loadPackage = function(metadata) {
  
      var PACKAGE_PATH = '';
      if (typeof window === 'object') {
        PACKAGE_PATH = window['encodeURIComponent'](window.location.pathname.toString().substring(0, window.location.pathname.toString().lastIndexOf('/')) + '/');
      } else if (typeof process === 'undefined' && typeof location !== 'undefined') {
        // web worker
        PACKAGE_PATH = encodeURIComponent(location.pathname.toString().substring(0, location.pathname.toString().lastIndexOf('/')) + '/');
      }
      var PACKAGE_NAME = 'vol-07.data';
      var REMOTE_PACKAGE_BASE = 'vol-07.data';
      if (typeof Module['locateFilePackage'] === 'function' && !Module['locateFile']) {
        Module['locateFile'] = Module['locateFilePackage'];
        err('warning: you defined Module.locateFilePackage, that has been renamed to Module.locateFile (using your locateFilePackage for now)');
      }
      var REMOTE_PACKAGE_NAME = Module['locateFile'] ? Module['locateFile'](REMOTE_PACKAGE_BASE, '') : REMOTE_PACKAGE_BASE;
    
      var REMOTE_PACKAGE_SIZE = metadata['remote_package_size'];
      var PACKAGE_UUID = metadata['package_uuid'];
    
      function fetchRemotePackage(packageName, packageSize, callback, errback) {
        
        var xhr = new XMLHttpRequest();
        xhr.open('GET', packageName, true);
        xhr.responseType = 'arraybuffer';
        xhr.onprogress = function(event) {
          var url = packageName;
          var size = packageSize;
          if (event.total) size = event.total;
          if (event.loaded) {
            if (!xhr.addedTotal) {
              xhr.addedTotal = true;
              if (!Module.dataFileDownloads) Module.dataFileDownloads = {};
              Module.dataFileDownloads[url] = {
                loaded: event.loaded,
                total: size
              };
            } else {
              Module.dataFileDownloads[url].loaded = event.loaded;
            }
            var total = 0;
            var loaded = 0;
            var num = 0;
            for (var download in Module.dataFileDownloads) {
            var data = Module.dataFileDownloads[download];
              total += data.total;
              loaded += data.loaded;
              num++;
            }
            total = Math.ceil(total * Module.expectedDataFileDownloads/num);
            if (Module['setStatus']) Module['setStatus']('Downloading data... (' + loaded + '/' + total + ')');
          } else if (!Module.dataFileDownloads) {
            if (Module['setStatus']) Module['setStatus']('Downloading data...');
          }
        };
        xhr.onerror = function(event) {
          throw new Error("NetworkError for: " + packageName);
        }
        xhr.onload = function(event) {
          if (xhr.status == 200 || xhr.status == 304 || xhr.status == 206 || (xhr.status == 0 && xhr.response)) { // file URLs can return 0
            var packageData = xhr.response;
            callback(packageData);
          } else {
            throw new Error(xhr.statusText + " : " + xhr.responseURL);
          }
        };
        xhr.send(null);
      };

      function handleError(error) {
        console.error('package error:', error);
      };
    
    function runWithFS() {
  
      function assert(check, msg) {
        if (!check) throw msg + new Error().stack;
      }
  Module['FS_createPath']("/", "assets", true, true);
Module['FS_createPath']("/assets", "sound", true, true);
Module['FS_createPath']("/assets", "shaders", true, true);
Module['FS_createPath']("/assets/shaders", "cubemap", true, true);
Module['FS_createPath']("/assets/shaders", "debug", true, true);
Module['FS_createPath']("/assets/shaders", "armature", true, true);
Module['FS_createPath']("/assets/shaders", "fog", true, true);
Module['FS_createPath']("/assets/shaders", "instanced", true, true);
Module['FS_createPath']("/assets/shaders", "particles", true, true);
Module['FS_createPath']("/assets/shaders", "water", true, true);
Module['FS_createPath']("/assets/shaders", "fish", true, true);

          /** @constructor */
          function DataRequest(start, end, audio) {
            this.start = start;
            this.end = end;
            this.audio = audio;
          }
          DataRequest.prototype = {
            requests: {},
            open: function(mode, name) {
              this.name = name;
              this.requests[name] = this;
              Module['addRunDependency']('fp ' + this.name);
            },
            send: function() {},
            onload: function() {
              var byteArray = this.byteArray.subarray(this.start, this.end);
              this.finish(byteArray);
            },
            finish: function(byteArray) {
              var that = this;
      
          Module['FS_createDataFile'](this.name, null, byteArray, true, true, true); // canOwn this data in the filesystem, it is a slide into the heap that will never change
          Module['removeRunDependency']('fp ' + that.name);
  
              this.requests[this.name] = null;
            }
          };
      
              var files = metadata['files'];
              for (var i = 0; i < files.length; ++i) {
                new DataRequest(files[i]['start'], files[i]['end'], files[i]['audio'] || 0).open('GET', files[i]['filename']);
              }
      
        
        var indexedDB;
        if (typeof window === 'object') {
          indexedDB = window.indexedDB || window.mozIndexedDB || window.webkitIndexedDB || window.msIndexedDB;
        } else if (typeof location !== 'undefined') {
          // worker
          indexedDB = self.indexedDB;
        } else {
          throw 'using IndexedDB to cache data can only be done on a web page or in a web worker';
        }
        var IDB_RO = "readonly";
        var IDB_RW = "readwrite";
        var DB_NAME = "EM_PRELOAD_CACHE";
        var DB_VERSION = 1;
        var METADATA_STORE_NAME = 'METADATA';
        var PACKAGE_STORE_NAME = 'PACKAGES';
        function openDatabase(callback, errback) {
          try {
            var openRequest = indexedDB.open(DB_NAME, DB_VERSION);
          } catch (e) {
            return errback(e);
          }
          openRequest.onupgradeneeded = function(event) {
            var db = event.target.result;

            if(db.objectStoreNames.contains(PACKAGE_STORE_NAME)) {
              db.deleteObjectStore(PACKAGE_STORE_NAME);
            }
            var packages = db.createObjectStore(PACKAGE_STORE_NAME);

            if(db.objectStoreNames.contains(METADATA_STORE_NAME)) {
              db.deleteObjectStore(METADATA_STORE_NAME);
            }
            var metadata = db.createObjectStore(METADATA_STORE_NAME);
          };
          openRequest.onsuccess = function(event) {
            var db = event.target.result;
            callback(db);
          };
          openRequest.onerror = function(error) {
            errback(error);
          };
        };

        // This is needed as chromium has a limit on per-entry files in IndexedDB
        // https://cs.chromium.org/chromium/src/content/renderer/indexed_db/webidbdatabase_impl.cc?type=cs&sq=package:chromium&g=0&l=177
        // https://cs.chromium.org/chromium/src/out/Debug/gen/third_party/blink/public/mojom/indexeddb/indexeddb.mojom.h?type=cs&sq=package:chromium&g=0&l=60
        // We set the chunk size to 64MB to stay well-below the limit
        var CHUNK_SIZE = 64 * 1024 * 1024;

        function cacheRemotePackage(
          db,
          packageName,
          packageData,
          packageMeta,
          callback,
          errback
        ) {
          var transactionPackages = db.transaction([PACKAGE_STORE_NAME], IDB_RW);
          var packages = transactionPackages.objectStore(PACKAGE_STORE_NAME);
          var chunkSliceStart = 0;
          var nextChunkSliceStart = 0;
          var chunkCount = Math.ceil(packageData.byteLength / CHUNK_SIZE);
          var finishedChunks = 0;
          for (var chunkId = 0; chunkId < chunkCount; chunkId++) {
            nextChunkSliceStart += CHUNK_SIZE;
            var putPackageRequest = packages.put(
              packageData.slice(chunkSliceStart, nextChunkSliceStart),
              'package/' + packageName + '/' + chunkId
            );
            chunkSliceStart = nextChunkSliceStart;
            putPackageRequest.onsuccess = function(event) {
              finishedChunks++;
              if (finishedChunks == chunkCount) {
                var transaction_metadata = db.transaction(
                  [METADATA_STORE_NAME],
                  IDB_RW
                );
                var metadata = transaction_metadata.objectStore(METADATA_STORE_NAME);
                var putMetadataRequest = metadata.put(
                  {
                    'uuid': packageMeta.uuid,
                    'chunkCount': chunkCount
                  },
                  'metadata/' + packageName
                );
                putMetadataRequest.onsuccess = function(event) {
                  callback(packageData);
                };
                putMetadataRequest.onerror = function(error) {
                  errback(error);
                };
              }
            };
            putPackageRequest.onerror = function(error) {
              errback(error);
            };
          }
        }

        /* Check if there's a cached package, and if so whether it's the latest available */
        function checkCachedPackage(db, packageName, callback, errback) {
          var transaction = db.transaction([METADATA_STORE_NAME], IDB_RO);
          var metadata = transaction.objectStore(METADATA_STORE_NAME);
          var getRequest = metadata.get('metadata/' + packageName);
          getRequest.onsuccess = function(event) {
            var result = event.target.result;
            if (!result) {
              return callback(false, null);
            } else {
              return callback(PACKAGE_UUID === result['uuid'], result);
            }
          };
          getRequest.onerror = function(error) {
            errback(error);
          };
        }

        function fetchCachedPackage(db, packageName, metadata, callback, errback) {
          var transaction = db.transaction([PACKAGE_STORE_NAME], IDB_RO);
          var packages = transaction.objectStore(PACKAGE_STORE_NAME);

          var chunksDone = 0;
          var totalSize = 0;
          var chunkCount = metadata['chunkCount'];
          var chunks = new Array(chunkCount);

          for (var chunkId = 0; chunkId < chunkCount; chunkId++) {
            var getRequest = packages.get('package/' + packageName + '/' + chunkId);
            getRequest.onsuccess = function(event) {
              // If there's only 1 chunk, there's nothing to concatenate it with so we can just return it now
              if (chunkCount == 1) {
                callback(event.target.result);
              } else {
                chunksDone++;
                totalSize += event.target.result.byteLength;
                chunks.push(event.target.result);
                if (chunksDone == chunkCount) {
                  if (chunksDone == 1) {
                    callback(event.target.result);
                  } else {
                    var tempTyped = new Uint8Array(totalSize);
                    var byteOffset = 0;
                    for (var chunkId in chunks) {
                      var buffer = chunks[chunkId];
                      tempTyped.set(new Uint8Array(buffer), byteOffset);
                      byteOffset += buffer.byteLength;
                      buffer = undefined;
                    }
                    chunks = undefined;
                    callback(tempTyped.buffer);
                    tempTyped = undefined;
                  }
                }
              }
            };
            getRequest.onerror = function(error) {
              errback(error);
            };
          }
        }
      
      function processPackageData(arrayBuffer) {
        assert(arrayBuffer, 'Loading data file failed.');
        assert(arrayBuffer instanceof ArrayBuffer, 'bad input to processPackageData');
        var byteArray = new Uint8Array(arrayBuffer);
        var curr;
        
          // Reuse the bytearray from the XHR as the source for file reads.
          DataRequest.prototype.byteArray = byteArray;
    
            var files = metadata['files'];
            for (var i = 0; i < files.length; ++i) {
              DataRequest.prototype.requests[files[i].filename].onload();
            }
                Module['removeRunDependency']('datafile_vol-07.data');

      };
      Module['addRunDependency']('datafile_vol-07.data');
    
      if (!Module.preloadResults) Module.preloadResults = {};
    
        function preloadFallback(error) {
          console.error(error);
          console.error('falling back to default preload behavior');
          fetchRemotePackage(REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE, processPackageData, handleError);
        };

        openDatabase(
          function(db) {
            checkCachedPackage(db, PACKAGE_PATH + PACKAGE_NAME,
              function(useCached, metadata) {
                Module.preloadResults[PACKAGE_NAME] = {fromCache: useCached};
                if (useCached) {
                  fetchCachedPackage(db, PACKAGE_PATH + PACKAGE_NAME, metadata, processPackageData, preloadFallback);
                } else {
                  fetchRemotePackage(REMOTE_PACKAGE_NAME, REMOTE_PACKAGE_SIZE,
                    function(packageData) {
                      cacheRemotePackage(db, PACKAGE_PATH + PACKAGE_NAME, packageData, {uuid:PACKAGE_UUID}, processPackageData,
                        function(error) {
                          console.error(error);
                          processPackageData(packageData);
                        });
                    }
                  , preloadFallback);
                }
              }
            , preloadFallback);
          }
        , preloadFallback);

        if (Module['setStatus']) Module['setStatus']('Downloading...');
      
    }
    if (Module['calledRun']) {
      runWithFS();
    } else {
      if (!Module['preRun']) Module['preRun'] = [];
      Module["preRun"].push(runWithFS); // FS is not initialized yet, wait for it
    }
  
   }
   loadPackage({"files": [{"filename": "/assets/sound/bg.raw", "start": 0, "end": 27818912}, {"filename": "/assets/sound/chestopen.raw", "start": 27818912, "end": 27853472}, {"filename": "/assets/sound/chestclosed.raw", "start": 27853472, "end": 27894560}, {"filename": "/assets/sound/coin.raw", "start": 27894560, "end": 27978572}, {"filename": "/assets/shaders/rock.frag", "start": 27978572, "end": 27985016}, {"filename": "/assets/shaders/shader_color.vert", "start": 27985016, "end": 27985421}, {"filename": "/assets/shaders/shader_tex.vert", "start": 27985421, "end": 27985887}, {"filename": "/assets/shaders/simplex.frag", "start": 27985887, "end": 27989148}, {"filename": "/assets/shaders/shader_tex.frag", "start": 27989148, "end": 27989691}, {"filename": "/assets/shaders/shader_color.frag", "start": 27989691, "end": 27989989}, {"filename": "/assets/shaders/coin.frag", "start": 27989989, "end": 27996571}, {"filename": "/assets/shaders/glow.frag", "start": 27996571, "end": 27997662}, {"filename": "/assets/shaders/simplex.vert", "start": 27997662, "end": 27997788}, {"filename": "/assets/shaders/chunk.frag", "start": 27997788, "end": 28005602}, {"filename": "/assets/shaders/chunk.vert", "start": 28005602, "end": 28007034}, {"filename": "/assets/shaders/glow.vert", "start": 28007034, "end": 28007149}, {"filename": "/assets/shaders/cubemap/cubemap.vert", "start": 28007149, "end": 28007359}, {"filename": "/assets/shaders/cubemap/cubemap.frag", "start": 28007359, "end": 28007532}, {"filename": "/assets/shaders/debug/simple_color_shader.frag", "start": 28007532, "end": 28007643}, {"filename": "/assets/shaders/debug/simple_uni_color_shader.vert", "start": 28007643, "end": 28007810}, {"filename": "/assets/shaders/debug/simple_color_shader.vert", "start": 28007810, "end": 28008058}, {"filename": "/assets/shaders/debug/simple_uni_color_shader.frag", "start": 28008058, "end": 28008185}, {"filename": "/assets/shaders/debug/simple_tex_shader.vert", "start": 28008185, "end": 28008423}, {"filename": "/assets/shaders/debug/simple_tex_shader.frag", "start": 28008423, "end": 28008587}, {"filename": "/assets/shaders/armature/shader_color_armature.vert", "start": 28008587, "end": 28010404}, {"filename": "/assets/shaders/armature/shadow.frag", "start": 28010404, "end": 28010536}, {"filename": "/assets/shaders/armature/shadow.vert", "start": 28010536, "end": 28011600}, {"filename": "/assets/shaders/armature/shader_color_armature.frag", "start": 28011600, "end": 28016877}, {"filename": "/assets/shaders/fog/underwaterFog.vert", "start": 28016877, "end": 28017102}, {"filename": "/assets/shaders/fog/underwaterFog.frag", "start": 28017102, "end": 28019006}, {"filename": "/assets/shaders/instanced/kelp.frag", "start": 28019006, "end": 28023964}, {"filename": "/assets/shaders/instanced/kelp_shadow.frag", "start": 28023964, "end": 28024188}, {"filename": "/assets/shaders/instanced/kelp.vert", "start": 28024188, "end": 28026093}, {"filename": "/assets/shaders/instanced/kelp_shadow.vert", "start": 28026093, "end": 28027099}, {"filename": "/assets/shaders/particles/bubbles.frag", "start": 28027099, "end": 28027798}, {"filename": "/assets/shaders/particles/bubbles.vert", "start": 28027798, "end": 28028332}, {"filename": "/assets/shaders/water/surface.frag", "start": 28028332, "end": 28029571}, {"filename": "/assets/shaders/water/caustics.frag", "start": 28029571, "end": 28030377}, {"filename": "/assets/shaders/water/caustics.vert", "start": 28030377, "end": 28032419}, {"filename": "/assets/shaders/water/surface.vert", "start": 28032419, "end": 28033220}, {"filename": "/assets/shaders/water/environment.frag", "start": 28033220, "end": 28033352}, {"filename": "/assets/shaders/water/environment.vert", "start": 28033352, "end": 28033739}, {"filename": "/assets/shaders/water/simulation.vert", "start": 28033739, "end": 28033984}, {"filename": "/assets/shaders/water/simulation.frag", "start": 28033984, "end": 28035171}, {"filename": "/assets/shaders/fish/pilotfish.frag", "start": 28035171, "end": 28040027}, {"filename": "/assets/shaders/fish/cubefish.vert", "start": 28040027, "end": 28041409}, {"filename": "/assets/shaders/fish/pilotfish.vert", "start": 28041409, "end": 28042980}, {"filename": "/assets/shaders/fish/shadow.frag", "start": 28042980, "end": 28043112}, {"filename": "/assets/shaders/fish/normal_fish.frag", "start": 28043112, "end": 28048058}, {"filename": "/assets/shaders/fish/cubefish.frag", "start": 28048058, "end": 28054660}, {"filename": "/assets/shaders/fish/shadow.vert", "start": 28054660, "end": 28055216}, {"filename": "/assets/shaders/fish/normal_fish.vert", "start": 28055216, "end": 28056775}], "remote_package_size": 28056775, "package_uuid": "b5250181-581b-4ade-9ef5-51868387b5ad"});
  
  })();
  