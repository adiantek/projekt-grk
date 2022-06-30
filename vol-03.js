
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
      var PACKAGE_NAME = 'vol-03.data';
      var REMOTE_PACKAGE_BASE = 'vol-03.data';
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
Module['FS_createPath']("/assets", "textures", true, true);
Module['FS_createPath']("/assets/textures", "entities", true, true);
Module['FS_createPath']("/assets/textures/entities", "wild_golden_fish", true, true);
Module['FS_createPath']("/assets/textures/entities", "robot", true, true);
Module['FS_createPath']("/assets/textures", "foliage", true, true);
Module['FS_createPath']("/assets/textures/foliage", "seagrass", true, true);
Module['FS_createPath']("/assets/textures/foliage", "kelp", true, true);
Module['FS_createPath']("/assets/textures", "generic", true, true);
Module['FS_createPath']("/assets/textures/generic", "Nature_Grass1_2x2_1K", true, true);

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
                Module['removeRunDependency']('datafile_vol-03.data');

      };
      Module['addRunDependency']('datafile_vol-03.data');
    
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
   loadPackage({"files": [{"filename": "/assets/textures/entities/wild_golden_fish/wild_golden_fish_normal.png", "start": 0, "end": 6224681}, {"filename": "/assets/textures/entities/robot/robot_propelers_Metalness.png", "start": 6224681, "end": 6279756}, {"filename": "/assets/textures/entities/robot/robot_body_Albedo.png", "start": 6279756, "end": 6654626}, {"filename": "/assets/textures/entities/robot/robot_propelers_AO.png", "start": 6654626, "end": 6714045}, {"filename": "/assets/textures/entities/robot/robot_chrome_Normal.png", "start": 6714045, "end": 7102902}, {"filename": "/assets/textures/entities/robot/robot_body_Normal.png", "start": 7102902, "end": 7162321}, {"filename": "/assets/textures/entities/robot/robot_metal_AO.png", "start": 7162321, "end": 7221740}, {"filename": "/assets/textures/entities/robot/robot_body_Roughness.png", "start": 7221740, "end": 7281160}, {"filename": "/assets/textures/entities/robot/robot_propelers_Normal.png", "start": 7281160, "end": 7878380}, {"filename": "/assets/textures/entities/robot/robot_wire_AO.png", "start": 7878380, "end": 7937799}, {"filename": "/assets/textures/entities/robot/robot_metal_Normal.png", "start": 7937799, "end": 7997218}, {"filename": "/assets/textures/entities/robot/robot_wire_Metalness.png", "start": 7997218, "end": 8056637}, {"filename": "/assets/textures/entities/robot/robot_wire_Normal.png", "start": 8056637, "end": 8116056}, {"filename": "/assets/textures/entities/robot/robot_metal_2_Roughness.png", "start": 8116056, "end": 8626514}, {"filename": "/assets/textures/entities/robot/robot_wire_Roughness.png", "start": 8626514, "end": 8685934}, {"filename": "/assets/textures/entities/robot/robot_metal_Metalness.png", "start": 8685934, "end": 8745354}, {"filename": "/assets/textures/entities/robot/robot_metal_Albedo.png", "start": 8745354, "end": 11891846}, {"filename": "/assets/textures/entities/robot/robot_lens_Albedo.png", "start": 11891846, "end": 12426851}, {"filename": "/assets/textures/entities/robot/robot_body_AO.png", "start": 12426851, "end": 12486270}, {"filename": "/assets/textures/entities/robot/robot_chrome_AO.png", "start": 12486270, "end": 12545689}, {"filename": "/assets/textures/entities/robot/robot_propelers_Roughness.png", "start": 12545689, "end": 13547332}, {"filename": "/assets/textures/entities/robot/robot_lens_AO.png", "start": 13547332, "end": 13606751}, {"filename": "/assets/textures/entities/robot/robot_metal_Roughness.png", "start": 13606751, "end": 17104302}, {"filename": "/assets/textures/entities/robot/robot_lens_Roughness.png", "start": 17104302, "end": 17481555}, {"filename": "/assets/textures/entities/robot/robot_body_Metalness.png", "start": 17481555, "end": 17536630}, {"filename": "/assets/textures/entities/robot/robot_lens_Normal.png", "start": 17536630, "end": 18204656}, {"filename": "/assets/textures/entities/robot/robot_glow_Normal.png", "start": 18204656, "end": 18578205}, {"filename": "/assets/textures/entities/robot/robot_chrome_Albedo.png", "start": 18578205, "end": 19078984}, {"filename": "/assets/textures/entities/robot/robot_propelers_Albedo.png", "start": 19078984, "end": 19261062}, {"filename": "/assets/textures/entities/robot/robot_glow_Metalness.png", "start": 19261062, "end": 19320481}, {"filename": "/assets/textures/entities/robot/robot_glow_Roughness.png", "start": 19320481, "end": 19695763}, {"filename": "/assets/textures/entities/robot/robot_glow_Albedo.png", "start": 19695763, "end": 20113182}, {"filename": "/assets/textures/entities/robot/robot_lens_Metalness.png", "start": 20113182, "end": 20168257}, {"filename": "/assets/textures/entities/robot/robot_wire_Albedo.png", "start": 20168257, "end": 20223332}, {"filename": "/assets/textures/entities/robot/robot_chrome_Roughness.png", "start": 20223332, "end": 20737093}, {"filename": "/assets/textures/entities/robot/robot_glow_AO.png", "start": 20737093, "end": 20796512}, {"filename": "/assets/textures/entities/robot/robot_metal_2_Albedo.png", "start": 20796512, "end": 21617236}, {"filename": "/assets/textures/entities/robot/robot_chrome_Metalness.png", "start": 21617236, "end": 22077723}, {"filename": "/assets/textures/entities/robot/robot_metal_2_Normal.png", "start": 22077723, "end": 22570804}, {"filename": "/assets/textures/entities/robot/robot_metal_2_AO.png", "start": 22570804, "end": 22630223}, {"filename": "/assets/textures/entities/robot/robot_metal_2_Metalness.png", "start": 22630223, "end": 22791165}, {"filename": "/assets/textures/foliage/seagrass/dried_grass_albedo.png", "start": 22791165, "end": 23475983}, {"filename": "/assets/textures/foliage/seagrass/grass_blades_normal.png", "start": 23475983, "end": 23885457}, {"filename": "/assets/textures/foliage/seagrass/grass_blades_albedo.png", "start": 23885457, "end": 24079092}, {"filename": "/assets/textures/foliage/seagrass/grass_blades_ao.png", "start": 24079092, "end": 24205342}, {"filename": "/assets/textures/foliage/seagrass/dried_grass_height.png", "start": 24205342, "end": 24669204}, {"filename": "/assets/textures/foliage/seagrass/dried_grass_normal.png", "start": 24669204, "end": 26144696}, {"filename": "/assets/textures/foliage/kelp/kelp_albedo.png", "start": 26144696, "end": 26936847}, {"filename": "/assets/textures/foliage/kelp/kelp_ao.png", "start": 26936847, "end": 27403731}, {"filename": "/assets/textures/foliage/kelp/kelp_normal.png", "start": 27403731, "end": 28506990}, {"filename": "/assets/textures/generic/Nature_Grass1_2x2_1K/Nature_Grass_1K_ao.png", "start": 28506990, "end": 31418334}, {"filename": "/assets/textures/generic/Nature_Grass1_2x2_1K/Nature_Grass_1K_height.png", "start": 31418334, "end": 35525645}, {"filename": "/assets/textures/generic/Nature_Grass1_2x2_1K/Nature_Grass_1K_roughness.png", "start": 35525645, "end": 37797214}, {"filename": "/assets/textures/generic/Nature_Grass1_2x2_1K/Nature_Grass_1K_albedo.png", "start": 37797214, "end": 47937804}], "remote_package_size": 47937804, "package_uuid": "fae5b08c-ecc9-49bd-a91b-d1cf60c6941b"});
  
  })();
  